#include "modelclass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_Texture = 0;
	m_model = 0;

	D3DXMatrixIdentity(&m_worldMatrix);
	D3DXMatrixIdentity(&m_scaling);
	D3DXMatrixIdentity(&m_rotation);
	D3DXMatrixIdentity(&m_translation);

	polygoneCount = 0;

	m_prePos = { 0,0,0 };
	m_pos = { 0,0,0 };
	m_rotate = { 0,0,0 };
	m_scale = { 1,1,1 };

	moveSpeed = 0.5f;
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, const WCHAR* textureFilename)
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

	// Load the texture for this model.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::Initialize(ID3D11Device* device, const WCHAR* textureFilename)
{
	bool result;	

	// Load the texture for this model.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	// Release the model texture. 
	ReleaseTexture();

	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

void ModelClass::SetModel(ModelType* model, int& vertexCount, int& indexCount)
{	
	m_vertexCount = vertexCount;
	m_indexCount = indexCount;
	m_model = model;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

D3DXMATRIX ModelClass::GetWorldMatrix()
{
	
	D3DXVECTOR3 rotate;

	rotate = m_rotate * 0.0174532925f;

	D3DXMatrixScaling(&m_scaling, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&m_rotation, rotate.y, rotate.x, rotate.z);
	D3DXMatrixTranslation(&m_translation, m_pos.x, m_pos.y, m_pos.z);

	m_worldMatrix = m_scaling * m_rotation * m_translation;
	return m_worldMatrix;
}

D3DXVECTOR3 ModelClass::GetPosition()
{
	return m_pos;
}

D3DXVECTOR3 ModelClass::GetRotation()
{
	return m_rotate;
}

void ModelClass::SetScaling(float x, float y, float z)
{
	m_scale = { x, y, z };
}

void ModelClass::SetRotation(float x, float y, float z)
{
	m_rotate = { x, y, z };
}

void ModelClass::SetPosition(float x, float y, float z)
{
	m_pos = { x, y, z };
}

int ModelClass::GetPolygonCount()
{
	return polygoneCount;
}

void ModelClass::Boost(bool input)
{
	if (m_prePos == m_pos)
	{
		moveSpeed = 0.0f;
		return;
	}


	if (input) {
		if(moveSpeed<3.05f)
		moveSpeed += 0.1f;
	}
	else
	{
		if (moveSpeed > 0.55f)
			moveSpeed -= 0.1f;
		else
		{
			moveSpeed += 0.1f;
		}
	}
}

void ModelClass::GoFoward()
{
	D3DXVECTOR3 lookAt, rotate;
	D3DXMATRIX rotationMatrix;


	moveDir[0] = true;
	moveDir[1] = false;
	moveDir[4] = true;
	moveRotX = 30.0f;

	lookAt = { 0,0,1 };

	rotate = m_rotate * 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotate.y, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3Normalize(&lookAt, &lookAt);

	m_pos = m_pos + lookAt * moveSpeed;
}

void ModelClass::GoBack()
{
	D3DXVECTOR3 lookAt, rotate;
	D3DXMATRIX rotationMatrix;
	moveDir[1] = true;
	moveDir[0] = false;
	moveDir[4] = true;
	moveRotX = -30.0f;
	lookAt = { 0,0,1 };

	rotate = m_rotate;
	rotate.y += 180;
	rotate *= 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotate.y, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3Normalize(&lookAt, &lookAt);

	m_pos = m_pos + lookAt * moveSpeed;
}

void ModelClass::GoLeft()
{
	D3DXVECTOR3 lookAt, rotate;
	D3DXMATRIX rotationMatrix;

	moveDir[2] = true;
	moveDir[3] = false;
	moveDir[4] = true;
	moveRotZ = 30.0f;

	lookAt = { 0,0,1 };

	rotate = m_rotate;
	rotate.y -= 90;
	rotate *= 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotate.y, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3Normalize(&lookAt, &lookAt);

	m_pos = m_pos + lookAt * moveSpeed;
}

void ModelClass::GoRight()
{
	D3DXVECTOR3 lookAt, rotate;
	D3DXMATRIX rotationMatrix;


	moveDir[3] = true;
	moveDir[2] = false;
	moveDir[4] = true;
	moveRotZ = -30.0f;

	lookAt = { 0,0,1 };

	rotate = m_rotate;
	rotate.y += 90;
	rotate *= 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotate.y,  0.0f, 0.0f);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3Normalize(&lookAt, &lookAt);

	m_pos = m_pos + lookAt * moveSpeed;
}

void ModelClass::GoUp()
{
	m_pos.y++;
}

void ModelClass::GoDown()
{
	m_pos.y--;
}

void ModelClass::RotateLeft()
{
	m_rotate.y -= 2.0f;
}

void ModelClass::RotateRight()
{
	m_rotate.y += 2.0f;
}

float ModelClass::GetMoveSpeed()
{
	return moveSpeed;
}

void ModelClass::SetMoveSpeed(float speed)
{
	moveSpeed = speed;
}

void ModelClass::DroneFrame(D3DXVECTOR3 prePos, D3DXVECTOR3 pos)
{
	m_prePos = prePos;
	m_rotate.x = moveRotX;
	m_rotate.z = moveRotZ;


	if (moveDir[0]) moveRotX -= 0.5f;
	if (moveDir[1]) moveRotX += 0.5f;
	if (moveDir[2]) moveRotZ -= 0.5f;
	if (moveDir[3]) moveRotZ += 0.5f;

	if (moveRotX >= -0.15f && moveRotX <= 0.15f) {
		moveRotX = 0.0f;
		moveDir[0] = false;
		moveDir[1] = false;
	}
	if (moveRotZ >= -0.15f && moveRotZ <= 0.15f) {
		moveRotZ = 0.0f;
		moveDir[2] = false;
		moveDir[3] = false;
	}

	if (moveRotX == 0 && moveRotZ == 0) moveDir[4] = false;

	return;
}

bool ModelClass::IsDroneMoved()
{
	return moveDir[4];
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}
	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	int i;
	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;

	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::ReadObjFileCount(char* filename)
{
	ifstream fin;
	char input;

	m_vertexCount = 0;
	m_textureCount = 0;
	m_normalCount = 0;
	m_faceCount = 0;
	m_faceNum = 0;
	bool faceNumRead = false;

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

			// '/'개수를 세서 사각형으로 그렸는지 삼각형으로 그렸는지 판별
			if (!faceNumRead) {
				while (input != '\n')
				{
					fin.get(input);
					if (input == '/')m_faceNum++;
				}
				// '/' 개수가 6개면 삼각형
				if (m_faceNum == 6) m_faceNum = 3;
				// '/' 개수가 8개면 사각형
				else if (m_faceNum == 8) m_faceNum = 4;
				faceNumRead = true;
			}
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	return true;
}

bool ModelClass::LoadModel(char* filename)
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
				if (m_faceNum == 4)
				{
					fin >> faces[faceIndex].vIndex4 >> input2 >> faces[faceIndex].tIndex4 >> input2 >> faces[faceIndex].nIndex4;
				}
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

	// 삼각형으로 그릴 때
	if (m_faceNum == 3)
	{
		m_vertexCount = (m_faceCount * 3);
	}
	// 사각형으로 그릴 때
	if (m_faceNum == 4)
	{
		m_vertexCount = (m_faceCount * 6);
		m_faceCount *= 2;
	}

	polygoneCount = m_faceCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	for (int i = 0; i < faceIndex; i++)
	{
		if (m_faceNum == 3)
		{
			vIndex = faces[i].vIndex1 - 1;
			tIndex = faces[i].tIndex1 - 1;
			nIndex = faces[i].nIndex1 - 1;

			m_model[i * 3].x = vertices[vIndex].x;
			m_model[i * 3].y = vertices[vIndex].y;
			m_model[i * 3].z = vertices[vIndex].z;
			m_model[i * 3].tu = texcoords[tIndex].x;
			m_model[i * 3].tv = texcoords[tIndex].y;
			m_model[i * 3].nx = normals[nIndex].x;
			m_model[i * 3].ny = normals[nIndex].y;
			m_model[i * 3].nz = normals[nIndex].z;

			vIndex = faces[i].vIndex2 - 1;
			tIndex = faces[i].tIndex2 - 1;
			nIndex = faces[i].nIndex2 - 1;

			m_model[i * 3 + 1].x = vertices[vIndex].x;
			m_model[i * 3 + 1].y = vertices[vIndex].y;
			m_model[i * 3 + 1].z = vertices[vIndex].z;
			m_model[i * 3 + 1].tu = texcoords[tIndex].x;
			m_model[i * 3 + 1].tv = texcoords[tIndex].y;
			m_model[i * 3 + 1].nx = normals[nIndex].x;
			m_model[i * 3 + 1].ny = normals[nIndex].y;
			m_model[i * 3 + 1].nz = normals[nIndex].z;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;

			m_model[i * 3 + 2].x = vertices[vIndex].x;
			m_model[i * 3 + 2].y = vertices[vIndex].y;
			m_model[i * 3 + 2].z = vertices[vIndex].z;
			m_model[i * 3 + 2].tu = texcoords[tIndex].x;
			m_model[i * 3 + 2].tv = texcoords[tIndex].y;
			m_model[i * 3 + 2].nx = normals[nIndex].x;
			m_model[i * 3 + 2].ny = normals[nIndex].y;
			m_model[i * 3 + 2].nz = normals[nIndex].z;
		}

		if (m_faceNum == 4)
		{
			vIndex = faces[i].vIndex1 - 1;
			tIndex = faces[i].tIndex1 - 1;
			nIndex = faces[i].nIndex1 - 1;

			m_model[i * 6].x = vertices[vIndex].x;
			m_model[i * 6].y = vertices[vIndex].y;
			m_model[i * 6].z = vertices[vIndex].z;
			m_model[i * 6].tu = texcoords[tIndex].x;
			m_model[i * 6].tv = texcoords[tIndex].y;
			m_model[i * 6].nx = normals[nIndex].x;
			m_model[i * 6].ny = normals[nIndex].y;
			m_model[i * 6].nz = normals[nIndex].z;

			vIndex = faces[i].vIndex2 - 1;
			tIndex = faces[i].tIndex2 - 1;
			nIndex = faces[i].nIndex2 - 1;

			m_model[i * 6 + 1].x = vertices[vIndex].x;
			m_model[i * 6 + 1].y = vertices[vIndex].y;
			m_model[i * 6 + 1].z = vertices[vIndex].z;
			m_model[i * 6 + 1].tu = texcoords[tIndex].x;
			m_model[i * 6 + 1].tv = texcoords[tIndex].y;
			m_model[i * 6 + 1].nx = normals[nIndex].x;
			m_model[i * 6 + 1].ny = normals[nIndex].y;
			m_model[i * 6 + 1].nz = normals[nIndex].z;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;

			m_model[i * 6 + 2].x = vertices[vIndex].x;
			m_model[i * 6 + 2].y = vertices[vIndex].y;
			m_model[i * 6 + 2].z = vertices[vIndex].z;
			m_model[i * 6 + 2].tu = texcoords[tIndex].x;
			m_model[i * 6 + 2].tv = texcoords[tIndex].y;
			m_model[i * 6 + 2].nx = normals[nIndex].x;
			m_model[i * 6 + 2].ny = normals[nIndex].y;
			m_model[i * 6 + 2].nz = normals[nIndex].z;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;

			m_model[i * 6 + 3].x = vertices[vIndex].x;
			m_model[i * 6 + 3].y = vertices[vIndex].y;
			m_model[i * 6 + 3].z = vertices[vIndex].z;
			m_model[i * 6 + 3].tu = texcoords[tIndex].x;
			m_model[i * 6 + 3].tv = texcoords[tIndex].y;
			m_model[i * 6 + 3].nx = normals[nIndex].x;
			m_model[i * 6 + 3].ny = normals[nIndex].y;
			m_model[i * 6 + 3].nz = normals[nIndex].z;

			vIndex = faces[i].vIndex4 - 1;
			tIndex = faces[i].tIndex4 - 1;
			nIndex = faces[i].nIndex4 - 1;

			m_model[i * 6 + 4].x = vertices[vIndex].x;
			m_model[i * 6 + 4].y = vertices[vIndex].y;
			m_model[i * 6 + 4].z = vertices[vIndex].z;
			m_model[i * 6 + 4].tu = texcoords[tIndex].x;
			m_model[i * 6 + 4].tv = texcoords[tIndex].y;
			m_model[i * 6 + 4].nx = normals[nIndex].x;
			m_model[i * 6 + 4].ny = normals[nIndex].y;
			m_model[i * 6 + 4].nz = normals[nIndex].z;

			vIndex = faces[i].vIndex1 - 1;
			tIndex = faces[i].tIndex1 - 1;
			nIndex = faces[i].nIndex1 - 1;

			m_model[i * 6 + 5].x = vertices[vIndex].x;
			m_model[i * 6 + 5].y = vertices[vIndex].y;
			m_model[i * 6 + 5].z = vertices[vIndex].z;
			m_model[i * 6 + 5].tu = texcoords[tIndex].x;
			m_model[i * 6 + 5].tv = texcoords[tIndex].y;
			m_model[i * 6 + 5].nx = normals[nIndex].x;
			m_model[i * 6 + 5].ny = normals[nIndex].y;
			m_model[i * 6 + 5].nz = normals[nIndex].z;
		}
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

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}