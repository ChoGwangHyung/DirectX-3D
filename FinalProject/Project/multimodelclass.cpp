#include "multimodelclass.h"

MultiModelClass::MultiModelClass()
{
	m_model = 0;
	m_lightShader = 0;
	m_models = 0;

	polygoneCount = 0;
}

MultiModelClass::MultiModelClass(const MultiModelClass& other)
{
}

MultiModelClass::~MultiModelClass()
{
}

bool MultiModelClass::Initialize(ID3D11Device* device, char* modelFilename, string texturePath)
{
	bool result;

	result = ReadObjFileCount(modelFilename);
	if (!result)
	{
		return false;
	}

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	for (int i = 0; i < m_modelCount; i++)
	{
		wstring temp = wstring(texturePath.begin(), texturePath.end()) + wstring(m_textureFileName[i].begin(), m_textureFileName[i].end()) + L".dds";
		const WCHAR* textureFileName = temp.c_str();

		result = m_models[i]->Initialize(device, textureFileName);
		if (!result)
		{
			return false;
		}
	}

	return true;
}

void MultiModelClass::Shutdown()
{
	// Release the model data.
	ReleaseModel();

	for (int i = 0; i < m_modelCount; i++)
		m_models[i]->Shutdown();

	m_lightShader = 0;

	return;
}

bool MultiModelClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR4 diffuseColor[], D3DXVECTOR4 lightPosition[], float range, D3DXVECTOR3 att, D3DXVECTOR4 directDiffColor, D3DXVECTOR4 directAmbColor, D3DXVECTOR3 cameraPos, D3DXVECTOR4 directSpecColor, float specularPower, D3DXVECTOR3 lightDir)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (int i = 0; i < m_modelCount; i++)
	{
		m_models[i]->Render(deviceContext);
		bool result = m_lightShader->Render(deviceContext, m_models[i]->GetIndexCount(), worldMatrix*m_models[i]->GetWorldMatrix(), viewMatrix, projectionMatrix, m_models[i]->GetTexture(), diffuseColor, lightPosition, range, att, directDiffColor, directAmbColor, cameraPos, directSpecColor, specularPower, lightDir);	
		if (!result)
		{
			return false;
		}
	}

	return true;
}

void MultiModelClass::SetScaling(float x, float y, float z)
{
	for (int i = 0; i < m_modelCount; i++)
		m_models[i]->SetScaling(x, y, z);
}

void MultiModelClass::SetRotation(float x, float y, float z)
{
	for (int i = 0; i < m_modelCount; i++)
		m_models[i]->SetRotation(x, y, z);
}

void MultiModelClass::SetPosition(float x, float y, float z)
{
	for (int i = 0; i < m_modelCount; i++)
		m_models[i]->SetPosition(x, y, z);
}

int MultiModelClass::GetPolygonCount()
{
	return polygoneCount;
}

void MultiModelClass::SetLightShaderClass(LightShaderClass* lightShader)
{
	m_lightShader = lightShader;
}

bool MultiModelClass::ReadObjFileCount(char* filename)
{
	ifstream fin;
	char input;

	m_vertexCount = 0;
	m_textureCount = 0;
	m_normalCount = 0;
	m_faceCount = 0;
	m_modelCount = 0;
	vector<pair<int, int>> insert;
	pair<int, int> insert2 = { 0,0 };
	m_eachModelFaceCount.clear();

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			switch (input)
			{
			case ' ':
				m_vertexCount++;
				break;
			case 't':
				m_textureCount++;
				break;
			case 'n':
				m_normalCount++;
				break;
			}
		}
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')	m_faceCount++;
		}

		if (input == 'u')
		{
			fin.get(input);
			if (input == 's')
			{				
				while (input != -100)
				{					
					fin.get(input);
				}
				fin.get(input);
				fin.get(input);

				string temp = "";

				//텍스쳐 파일 이름 넣기
				while (input != '\n')
				{
					temp += input;
					fin.get(input);
				}

				m_textureFileName.push_back(temp);
				m_eachModelFaceCount.push_back(insert);
				m_modelCount++;

				insert2.second = m_faceCount - 1;
				if (m_faceCount != 0)
				{
					m_eachModelFaceCount[m_modelCount - 2].push_back(insert2);
					insert2.first = insert2.second;
				}
			}
		}
		insert.clear();

		while (input != '\n')
		{
			fin.get(input);
		}		

		fin.get(input);
	}

	insert2.second = m_faceCount - 1;
	m_eachModelFaceCount[m_modelCount - 1].push_back(insert2);

	for (int i = 0; i < m_textureFileName.size(); i++)
	{
		if (m_textureFileName[i] == "FrontColor")
		{
			if (i == 0)
				m_textureFileName[i] = m_textureFileName[i + 1];
			else
				m_textureFileName[i] = m_textureFileName[i - 1];
		}

	}

	fin.close();

	return true;
}

bool MultiModelClass::LoadModel(char* filename)
{
	ifstream fin;
	FaceType* faces;
	vertexType *vertices, *texcoords, *normals;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;

	vertices = new vertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new vertexType[m_textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new vertexType[m_normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[m_faceCount];
	if (!faces)
	{
		return false;
	}

	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	fin.get(input);

	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;
				//오른손 좌표계에서 왼손 좌표계 변환
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f; vertexIndex++;
			}
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;
				//오른손 좌표계에서 왼손 좌표계 변환
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y; texcoordIndex++;
			}
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;
				//오른손 좌표계에서 왼손 좌표계 변환
				normals[normalIndex].z = normals[normalIndex].z * -1.0f; normalIndex++;
			}
		}

		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3 >>
					faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2 >>
					faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	polygoneCount = m_faceCount;

	m_models = new ModelClass*[m_modelCount];
	if (!m_models)
	{
		return false;
	}

	m_model = new ModelClass::ModelType*[m_modelCount];
	if (!m_model)
	{
		return false;
	}

	for (int i = 0; i < m_modelCount; i++)
	{
		//vertexCount 계산
		int vCount = 0;
		for (int j = 0; j < m_eachModelFaceCount[i].size(); j++)
			vCount += ((m_eachModelFaceCount[i][j].second - m_eachModelFaceCount[i][j].first + 1) * 3);

		//초기화
		m_model[i] = new ModelClass::ModelType[vCount];
		if (!m_model[i])
		{
			return false;
		}

		m_models[i] = new ModelClass;
		if (!m_models[i])
		{
			return false;
		}

		//받아오기		
		int j = 0;
		for (int r = 0; r < m_eachModelFaceCount[i].size(); r++)
		{
			for (int eachModelFaceIndex = m_eachModelFaceCount[i][r].first; eachModelFaceIndex <= m_eachModelFaceCount[i][r].second; eachModelFaceIndex++)
			{
				vIndex = faces[eachModelFaceIndex].vIndex1 - 1;
				tIndex = faces[eachModelFaceIndex].tIndex1 - 1;
				nIndex = faces[eachModelFaceIndex].nIndex1 - 1;

				m_model[i][j * 3].x = vertices[vIndex].x;
				m_model[i][j * 3].y = vertices[vIndex].y;
				m_model[i][j * 3].z = vertices[vIndex].z;
				m_model[i][j * 3].tu = texcoords[tIndex].x;
				m_model[i][j * 3].tv = texcoords[tIndex].y;
				m_model[i][j * 3].nx = normals[nIndex].x;
				m_model[i][j * 3].ny = normals[nIndex].y;
				m_model[i][j * 3].nz = normals[nIndex].z;

				vIndex = faces[eachModelFaceIndex].vIndex2 - 1;
				tIndex = faces[eachModelFaceIndex].tIndex2 - 1;
				nIndex = faces[eachModelFaceIndex].nIndex2 - 1;

				m_model[i][j * 3 + 1].x = vertices[vIndex].x;
				m_model[i][j * 3 + 1].y = vertices[vIndex].y;
				m_model[i][j * 3 + 1].z = vertices[vIndex].z;
				m_model[i][j * 3 + 1].tu = texcoords[tIndex].x;
				m_model[i][j * 3 + 1].tv = texcoords[tIndex].y;
				m_model[i][j * 3 + 1].nx = normals[nIndex].x;
				m_model[i][j * 3 + 1].ny = normals[nIndex].y;
				m_model[i][j * 3 + 1].nz = normals[nIndex].z;

				vIndex = faces[eachModelFaceIndex].vIndex3 - 1;
				tIndex = faces[eachModelFaceIndex].tIndex3 - 1;
				nIndex = faces[eachModelFaceIndex].nIndex3 - 1;

				m_model[i][j * 3 + 2].x = vertices[vIndex].x;
				m_model[i][j * 3 + 2].y = vertices[vIndex].y;
				m_model[i][j * 3 + 2].z = vertices[vIndex].z;
				m_model[i][j * 3 + 2].tu = texcoords[tIndex].x;
				m_model[i][j * 3 + 2].tv = texcoords[tIndex].y;
				m_model[i][j * 3 + 2].nx = normals[nIndex].x;
				m_model[i][j * 3 + 2].ny = normals[nIndex].y;
				m_model[i][j * 3 + 2].nz = normals[nIndex].z;

				j++;
			}
		}

		m_models[i]->SetModel(m_model[i], vCount, vCount);
	}


	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}

	if (normals)
	{
		delete[] normals;
		normals = 0;
	}

	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}

void MultiModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}