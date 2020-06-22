#pragma once

#include <d3d11.h>
#include <d3dx10math.h> 
#include "textureclass.h"

class SkyPlaneClass
{
private:
	struct SkyPlaneType
	{
		float x, y, z;
		float tu, tv;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	SkyPlaneClass();
	SkyPlaneClass(const SkyPlaneClass&);
	~SkyPlaneClass();

	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	void Frame();

	int GetIndexCount();
	ID3D11ShaderResourceView* GetCloudTexture1();
	ID3D11ShaderResourceView* GetCloudTexture2();

	void SetBrightness(float);
	float GetBrightness();
	float GetTranslation(int);

private:
	bool InitializeSkyPlane(int, float, float, float, int);
	void ShutdownSkyPlane();

	bool InitializeBuffers(ID3D11Device*, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, const WCHAR*, const WCHAR*);
	void ReleaseTextures();

private:
	SkyPlaneType* m_skyPlane = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	ID3D11Buffer *m_vertexBuffer = nullptr;
	ID3D11Buffer *m_indexBuffer = nullptr;
	TextureClass *m_CloudTexture1 = nullptr;
	TextureClass *m_CloudTexture2 = nullptr;
	float m_brightness = 0.0f;
	float m_translationSpeed[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float m_textureTranslation[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
};

