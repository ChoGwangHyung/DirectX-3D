#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;

const int NUM_LIGHTS = 26;

class LightShaderClass
{


	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct LightColorBufferType
	{
		D3DXVECTOR4 diffuseColor[NUM_LIGHTS];
		D3DXVECTOR4 directDiffColor;
		D3DXVECTOR4 directAmbColor;
		D3DXVECTOR4 directSpecColor;
		float specularPower;
		D3DXVECTOR3 lightDirection;
		float range;
		D3DXVECTOR3 att;
	};

	struct LightPositionBufferType
	{
		D3DXVECTOR4 lightPosition[NUM_LIGHTS];
		D3DXVECTOR3 cameraPosition;
		float padding;
	};
public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,
				ID3D11ShaderResourceView*, D3DXVECTOR4[], D3DXVECTOR4[], float, D3DXVECTOR3,
				D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float, D3DXVECTOR3);

	bool SetFilter(ID3D11Device*, D3D11_FILTER);
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,
							 ID3D11ShaderResourceView*, D3DXVECTOR4[], D3DXVECTOR4[], float, D3DXVECTOR3,
							 D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float, D3DXVECTOR3);

	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState * m_sampleState;


	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightColorBuffer;
	ID3D11Buffer* m_lightPositionBuffer;

};

