#pragma once

#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>

#include "textureclass.h"
using namespace std;

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv; //텍스쳐 좌표
		float nx, ny, nz; //법선 벡터
	};

	struct vertexType
	{
		float x, y, z;
	};

	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3, vIndex4; //정점
		int tIndex1, tIndex2, tIndex3, tIndex4; //텍스쳐
		int nIndex1, nIndex2, nIndex3, nIndex4; //법선
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	D3DXMATRIX GetWorldMatrix();
	void SetScaling(float, float, float);
	void SetRotation(char, float);
	void SetTranslation(float, float, float);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool ReadObjFileCount(char*);
	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount, m_textureCount, m_normalCount, m_faceCount;
	int m_faceNum;

	TextureClass* m_Texture;
	ModelType* m_model;

	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_scaling;
	D3DXMATRIX m_rotation;
	D3DXMATRIX m_translation;
};
#endif