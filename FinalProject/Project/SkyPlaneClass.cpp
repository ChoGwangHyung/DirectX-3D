#include "SkyPlaneClass.h"




SkyPlaneClass::SkyPlaneClass()
{
}


SkyPlaneClass::SkyPlaneClass(const SkyPlaneClass& other)
{
}


SkyPlaneClass::~SkyPlaneClass()
{
}


bool SkyPlaneClass::Initialize(ID3D11Device* device, const WCHAR* textureFilename1, const WCHAR* textureFilename2)
{
	// 하늘 평면 매개 변수를 설정합니다.
	int skyPlaneResolution = 10;
	int textureRepeat = 4;
	float skyPlaneWidth = 10.0f;
	float skyPlaneTop = 0.5f;
	float skyPlaneBottom = 0.0f;

	// 구름의 밝기를 설정합니다.
	m_brightness = 0.65f;

	// 구름 변환 속도 증분을 설정합니다.
	m_translationSpeed[0] = 0.0003f;   // 첫 번째 텍스처 X 변환 속도.
	m_translationSpeed[1] = 0.0f;      // 첫 번째 텍스처 Z 변환 속도.
	m_translationSpeed[2] = 0.00015f;  // 두 번째 텍스처 X 변환 속도.
	m_translationSpeed[3] = 0.0f;      // 두 번째 텍스처 Z 변환 속도.

	// 텍스처 변환 값을 초기화합니다.
	m_textureTranslation[0] = 0.0f;
	m_textureTranslation[1] = 0.0f;
	m_textureTranslation[2] = 0.0f;
	m_textureTranslation[3] = 0.0f;

	// 하늘 평면을 만듭니다.
	bool result = InitializeSkyPlane(skyPlaneResolution, skyPlaneWidth, skyPlaneTop, skyPlaneBottom, textureRepeat);
	if(!result)
	{
		return false;
	}

	// 하늘 평면에 대한 정점 및 인덱스 버퍼를 만듭니다.
	result = InitializeBuffers(device, skyPlaneResolution);
	if(!result)
	{
		return false;
	}

	// 하늘 평면 텍스처를 로드합니다.
	result = LoadTextures(device, textureFilename1, textureFilename2);
	if(!result)
	{
		return false;
	}

	return true;
}


void SkyPlaneClass::Shutdown()
{
	// 하늘 평면 텍스처를 해제합니다.
	ReleaseTextures();

	// 하늘 평면 렌더링에 사용 된 정점 및 인덱스 버퍼를 해제합니다.
	ShutdownBuffers();

	// 하늘 평면 배열을 해제합니다.
	ShutdownSkyPlane();
}


void SkyPlaneClass::Render(ID3D11DeviceContext* deviceContext)
{
	// 하늘 평면을 렌더링합니다.
	RenderBuffers(deviceContext);
}


void SkyPlaneClass::Frame()
{
	// 움직이는 구름을 시뮬레이트하기 위해 변환 값을 증가시킵니다.
	m_textureTranslation[0] += m_translationSpeed[0];
	m_textureTranslation[1] += m_translationSpeed[1];
	m_textureTranslation[2] += m_translationSpeed[2];
	m_textureTranslation[3] += m_translationSpeed[3];

	// 값을 0에서 1 범위로 유지합니다.
	if(m_textureTranslation[0] > 1.0f)  {  m_textureTranslation[0] -= 1.0f;  }
	if(m_textureTranslation[1] > 1.0f)  {  m_textureTranslation[1] -= 1.0f;  }
	if(m_textureTranslation[2] > 1.0f)  {  m_textureTranslation[2] -= 1.0f;  }
	if(m_textureTranslation[3] > 1.0f)  {  m_textureTranslation[3] -= 1.0f;  }
}


int SkyPlaneClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* SkyPlaneClass::GetCloudTexture1()
{
	return m_CloudTexture1->GetTexture();
}


ID3D11ShaderResourceView* SkyPlaneClass::GetCloudTexture2()
{
	return m_CloudTexture2->GetTexture();
}


void SkyPlaneClass::SetBrightness(float brightness)
{
	m_brightness = brightness;
}

float SkyPlaneClass::GetBrightness()
{
	return m_brightness;
}


float SkyPlaneClass::GetTranslation(int index)
{
	return m_textureTranslation[index];
}


bool SkyPlaneClass::InitializeSkyPlane(int skyPlaneResolution, float skyPlaneWidth, float skyPlaneTop, float skyPlaneBottom, int textureRepeat)
{
	float positionX = 0.0f;
	float positionY = 0.0f;
	float positionZ = 0.0f;
	float tu = 0.0f;
	float tv = 0.0f;

	// 하늘 평면 좌표를 보유 할 배열을 만듭니다.
	m_skyPlane = new SkyPlaneType[(skyPlaneResolution + 1) * (skyPlaneResolution + 1)];
	if(!m_skyPlane)
	{
		return false;
	}

	// 하늘 평면에서 각 쿼드의 크기를 결정합니다.
	float quadSize = skyPlaneWidth / (float)skyPlaneResolution;

	// 너비를 기준으로 하늘 평면의 반지름을 계산합니다.
	float radius = skyPlaneWidth / 2.0f;

	// 증가 할 높이 상수를 계산합니다.
	float constant = (skyPlaneTop - skyPlaneBottom) / (radius * radius);

	// 텍스처 좌표 증가 값을 계산합니다.
	float textureDelta = (float)textureRepeat / (float)skyPlaneResolution;

	// 하늘 평면을 반복하고 제공된 증분 값을 기반으로 좌표를 만듭니다.
	for(int j=0; j<=skyPlaneResolution; j++)
	{
		for(int i=0; i<=skyPlaneResolution; i++)
		{
			// 정점 좌표를 계산합니다.
			positionX = (-0.5f * skyPlaneWidth) + ((float)i * quadSize);
			positionZ = (-0.5f * skyPlaneWidth) + ((float)j * quadSize);
			positionY = skyPlaneTop - (constant * ((positionX * positionX) + (positionZ * positionZ)));

			// 텍스처 좌표를 계산합니다.
			tu = (float)i * textureDelta;
			tv = (float)j * textureDelta;

			// 이 좌표를 추가하기 위해 하늘 평면 배열에 인덱스를 계산합니다.
			int index = j * (skyPlaneResolution + 1) + i;

			// 하늘 평면 배열에 좌표를 추가합니다.
			m_skyPlane[index].x = positionX;
			m_skyPlane[index].y = positionY;
			m_skyPlane[index].z = positionZ;
			m_skyPlane[index].tu = tu;
			m_skyPlane[index].tv = tv;
		}
	}

	return true;
}


void SkyPlaneClass::ShutdownSkyPlane()
{
	// 하늘 평면 배열을 해제합니다.
	if(m_skyPlane)
	{
		delete [] m_skyPlane;
		m_skyPlane = 0;
	}
}


bool SkyPlaneClass::InitializeBuffers(ID3D11Device* device, int skyPlaneResolution)
{
	int index1 = 0;
	int index2 = 0;
	int index3 = 0;
	int index4 = 0;

	// 하늘 평면 메쉬의 정점 수를 계산합니다.
	m_vertexCount = (skyPlaneResolution + 1) * (skyPlaneResolution + 1) * 6;

	// 인덱스 수를 꼭지점 수와 같게 설정합니다.
	m_indexCount = m_vertexCount;
		
	// 정점 배열을 만듭니다.
	VertexType* vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// 인덱스 배열을 만듭니다.
	unsigned long* indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// 인덱스를 정점 배열로 초기화 합니다.
	int index = 0;

	// 하늘 평면 배열 데이터로 꼭지점과 인덱스 배열을 로드합니다.
	for(int j=0; j<skyPlaneResolution; j++)
	{
		for(int i=0; i<skyPlaneResolution; i++)
		{
			index1 = j * (skyPlaneResolution + 1) + i;
			index2 = j * (skyPlaneResolution + 1) + (i+1);
			index3 = (j+1) * (skyPlaneResolution + 1) + i;
			index4 = (j+1) * (skyPlaneResolution + 1) + (i+1);

			// 삼각형 1 - 왼쪽 위
			vertices[index].position = D3DXVECTOR3(m_skyPlane[index1].x, m_skyPlane[index1].y, m_skyPlane[index1].z);
			vertices[index].texture = D3DXVECTOR2(m_skyPlane[index1].tu, m_skyPlane[index1].tv);
			indices[index] = index;
			index++;

			// 삼각형 1 - 오른쪽 위
			vertices[index].position = D3DXVECTOR3(m_skyPlane[index2].x, m_skyPlane[index2].y, m_skyPlane[index2].z);
			vertices[index].texture = D3DXVECTOR2(m_skyPlane[index2].tu, m_skyPlane[index2].tv);
			indices[index] = index;
			index++;

			// 삼각형 1 - 왼쪽 아래
			vertices[index].position = D3DXVECTOR3(m_skyPlane[index3].x, m_skyPlane[index3].y, m_skyPlane[index3].z);
			vertices[index].texture = D3DXVECTOR2(m_skyPlane[index3].tu, m_skyPlane[index3].tv);
			indices[index] = index;
			index++;

			// 삼각형 2 - 왼쪽 아래
			vertices[index].position = D3DXVECTOR3(m_skyPlane[index3].x, m_skyPlane[index3].y, m_skyPlane[index3].z);
			vertices[index].texture = D3DXVECTOR2(m_skyPlane[index3].tu, m_skyPlane[index3].tv);
			indices[index] = index;
			index++;

			// 삼각형 2 - 오른쪽 위
			vertices[index].position = D3DXVECTOR3(m_skyPlane[index2].x, m_skyPlane[index2].y, m_skyPlane[index2].z);
			vertices[index].texture = D3DXVECTOR2(m_skyPlane[index2].tu, m_skyPlane[index2].tv);
			indices[index] = index;
			index++;

			// 삼각형 2 - 오른쪽 아래
			vertices[index].position = D3DXVECTOR3(m_skyPlane[index4].x, m_skyPlane[index4].y, m_skyPlane[index4].z);
			vertices[index].texture = D3DXVECTOR2(m_skyPlane[index4].tu, m_skyPlane[index4].tv);
			indices[index] = index;
			index++;
		}
	}

	// 정점 버퍼의 구조체를 설정한다.
	D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 정점 버퍼를 만듭니다.
	if(FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// 인덱스 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 하위 리소스 구조에 인덱스 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 만듭니다.
	if(FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// 버텍스와 인덱스 버퍼가 생성되고 로드된 배열을 해제합니다.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void SkyPlaneClass::ShutdownBuffers()
{
	// 인덱스 버퍼를 해제합니다.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 버텍스 버퍼를 해제합니다.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


void SkyPlaneClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼 보폭 및 오프셋을 설정합니다.
    unsigned int stride = sizeof(VertexType); 
	unsigned int offset = 0;
    
	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // 이 꼭지점 버퍼에서 렌더링되어야하는 프리미티브 유형을 설정합니다.이 경우에는 삼각형입니다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool SkyPlaneClass::LoadTextures(ID3D11Device* device, const WCHAR* textureFilename1, const WCHAR* textureFilename2)
{
	// 첫 번째 클라우드 텍스처 객체를 만듭니다.
	m_CloudTexture1 = new TextureClass;
	if(!m_CloudTexture1)
	{
		return false;
	}

	// 첫 번째 구름 텍스처 객체를 초기화합니다.
	if(!m_CloudTexture1->Initialize(device, const_cast<WCHAR*>(textureFilename1)))
	{
		return false;
	}

	// 두 번째 클라우드 텍스처 객체를 만듭니다.
	m_CloudTexture2 = new TextureClass;
	if(!m_CloudTexture2)
	{
		return false;
	}

	// 두 번째 구름 텍스처 객체를 초기화합니다.
	if(!m_CloudTexture2->Initialize(device, const_cast<WCHAR*>(textureFilename2)))
	{
		return false;
	}

	return true;
}


void SkyPlaneClass::ReleaseTextures()
{
	// 텍스처 객체를 해제합니다.
	if(m_CloudTexture1)
	{
		m_CloudTexture1->Shutdown();
		delete m_CloudTexture1;
		m_CloudTexture1 = 0;
	}

	if(m_CloudTexture2)
	{
		m_CloudTexture2->Shutdown();
		delete m_CloudTexture2;
		m_CloudTexture2 = 0;
	}
}