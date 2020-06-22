#pragma once

#include <d3d11.h>
#include <d3dx10math.h> 


class SkyDomeClass
{
private:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	};

public:
	SkyDomeClass();
	SkyDomeClass(const SkyDomeClass&);
	~SkyDomeClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	D3DXVECTOR4 GetApexColor();
	D3DXVECTOR4 GetCenterColor();

	void Frame();
	void SkyChange();
	bool IsSpecularLite();
	int GetTime();

private:
	bool LoadSkyDomeModel(const char*);
	void ReleaseSkyDomeModel();

	bool InitializeBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ModelType* m_model = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	D3DXVECTOR4 m_apexColor;
	D3DXVECTOR4 m_centerColor;

	D3DXVECTOR4 sunsetApexColor;
	D3DXVECTOR4 sunsetCenterColor;
	D3DXVECTOR4 nightApexColor;
	D3DXVECTOR4 nightCenterColor;
	D3DXVECTOR4 dayApexColor;
	D3DXVECTOR4 dayCenterColor;

	D3DXVECTOR4 dayToSunsetApex;
	D3DXVECTOR4 dayToSunsetCenter;
	D3DXVECTOR4 sunsetToDayApex;
	D3DXVECTOR4 sunsetToDayCenter;

	D3DXVECTOR4 nightToSunsetApex;
	D3DXVECTOR4 nightToSunsetCenter;
	D3DXVECTOR4 sunsetToNightApex;
	D3DXVECTOR4 sunsetToNightCenter;

	int time;
};

