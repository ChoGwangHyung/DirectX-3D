#include "minimapclass.h"

MiniMapClass::MiniMapClass()
{
	m_mapLocationX = 0;
	m_mapLocationY = 0;
	m_pointLocationX = 0;
	m_pointLocationY = 0;
	m_mapSizeX = 0;
	m_mapSizeY = 0;
	m_terrainXLeftBoundary = 0;
	m_terrainZLeftBoundary = 0;
	m_terrainXRightBoundary = 0;
	m_terrainZRightBoundary = 0;
	m_MiniMapBitmap = 0;
	m_Border = 0;
	m_Point = 0;
}

MiniMapClass::MiniMapClass(const MiniMapClass& other)
{
}

MiniMapClass::~MiniMapClass()
{
}

bool MiniMapClass::Initialize(ID3D11Device* device, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX viewMatrix, float terrainXLeftBoundary, float terrainXRightBoundary, float terrainZLeftBoundary, float terrainZRightBoundary)
{
	// 화면에서 미니 맵의 위치를 ​​초기화합니다.
	m_mapLocationX = 630;
	m_mapLocationY = 430;

	// 미니 맵의 크기를 설정합니다.
	m_mapSizeX = 150.0f;
	m_mapSizeY = 150.0f;

	// 기본 뷰 매트릭스를 저장합니다.
	m_viewMatrix = viewMatrix;

	// 지형 크기를 저장합니다.
	m_terrainXLeftBoundary = terrainXLeftBoundary;
	m_terrainXRightBoundary = terrainXRightBoundary;
	m_terrainZLeftBoundary = terrainZLeftBoundary;
	m_terrainZRightBoundary = terrainZRightBoundary;

	// 미니 맵 비트 맵 객체를 만듭니다.
	m_MiniMapBitmap = new BitmapClass;
	if (!m_MiniMapBitmap)
	{
		return false;
	}

	// 미니 맵 비트 맵 객체를 초기화합니다.
	bool result = m_MiniMapBitmap->Initialize(device, screenWidth, screenHeight, L"../Project/data/map.dds", 150, 150);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the mini-map object.", L"Error", MB_OK);
		return false;
	}

	// 테두리 비트 맵 객체를 만듭니다.
	m_Border = new BitmapClass;
	if (!m_Border)
	{
		return false;
	}

	// 테두리 비트 맵 객체를 초기화합니다.
	result = m_Border->Initialize(device, screenWidth, screenHeight, L"../Project/data/border.dds", 154, 154);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the border object.", L"Error", MB_OK);
		return false;
	}

	// 포인트 비트 맵 객체를 만듭니다.
	m_Point = new BitmapClass;
	if (!m_Point)
	{
		return false;
	}

	// 포인트 비트 맵 객체를 초기화합니다.
	result = m_Point->Initialize(device, screenWidth, screenHeight, L"../Project/data/point.dds", 5, 5);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the point object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void MiniMapClass::Shutdown()
{
	// 포인트 비트 맵 객체를 해제합니다.
	if (m_Point)
	{
		m_Point->Shutdown();
		delete m_Point;
		m_Point = 0;
	}

	// 테두리 비트 맵 객체를 해제합니다.
	if (m_Border)
	{
		m_Border->Shutdown();
		delete m_Border;
		m_Border = 0;
	}

	// 미니 맵 비트 맵 객체를 해제합니다.
	if (m_MiniMapBitmap)
	{
		m_MiniMapBitmap->Shutdown();
		delete m_MiniMapBitmap;
		m_MiniMapBitmap = 0;
	}
}

bool MiniMapClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix, TextureShaderClass* textureShader)
{
	// 테두리 비트 맵 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	bool result = m_Border->Render(deviceContext, (m_mapLocationX - 2), (m_mapLocationY - 2));
	if (!result)
	{
		return false;
	}

	// 텍스처 셰이더를 사용하여 테두리 비트 맵을 렌더링합니다.
	textureShader->Render(deviceContext, m_Border->GetIndexCount(), worldMatrix, m_viewMatrix, orthoMatrix, m_Border->GetTexture());

	// 미니 맵 비트 맵 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	result = m_MiniMapBitmap->Render(deviceContext, m_mapLocationX, m_mapLocationY);
	if (!result)
	{
		return false;
	}

	// 텍스처 셰이더를 사용하여 미니 맵 비트 맵을 렌더링합니다.
	textureShader->Render(deviceContext, m_MiniMapBitmap->GetIndexCount(), worldMatrix, m_viewMatrix, orthoMatrix, m_MiniMapBitmap->GetTexture());

	// 포인트 비트 맵 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	result = m_Point->Render(deviceContext, m_pointLocationX, m_pointLocationY);
	if (!result)
	{
		return false;
	}

	// 텍스처 셰이더를 사용하여 포인트 비트 맵을 렌더링합니다.
	textureShader->Render(deviceContext, m_Point->GetIndexCount(), worldMatrix, m_viewMatrix, orthoMatrix, m_Point->GetTexture());

	return true;
}

void MiniMapClass::PositionUpdate(float positionX, float positionZ)
{
	// 카메라가 지형 경계선을 지나쳐도 포인트가 미니 맵 테두리를 떠나지 않는지 확인합니다.
	if (positionX < m_terrainXLeftBoundary)
	{
		positionX = m_terrainXLeftBoundary;
	}

	if (positionZ < m_terrainZLeftBoundary)
	{
		positionZ = m_terrainZLeftBoundary;
	}

	if (positionX > m_terrainXRightBoundary)
	{
		positionX = m_terrainXRightBoundary;
	}

	if (positionZ > m_terrainZRightBoundary)
	{
		positionZ = m_terrainZRightBoundary;
	}

	// 미니 맵에서 카메라의 위치를 ​​백분율로 계산합니다.
	float percentX = positionX / (m_terrainXLeftBoundary - m_terrainXRightBoundary);
	float percentY = 1.0f - (positionZ / (m_terrainZLeftBoundary - m_terrainZRightBoundary));

	// 미니 맵에서 포인트의 픽셀 위치를 결정합니다.
	m_pointLocationX = m_mapLocationX + 75 + (int)(percentX * m_mapSizeX);
	m_pointLocationY = m_mapLocationY - 75 + (int)(percentY * m_mapSizeY);

	// 3x3 포인트 픽셀 이미지 크기에 따라 미니 맵에서 포인트의 중심에 위치에서 1을 뺍니다.
	m_pointLocationX = m_pointLocationX - 1;
	m_pointLocationY = m_pointLocationY - 1;
}