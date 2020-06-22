#pragma once

#ifndef _MULTIMODELCLASS_H_
#define _MULTIMODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include <vector>

#include "textureclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
using namespace std;

class MultiModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
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
	MultiModelClass();
	MultiModelClass(const MultiModelClass&);
	~MultiModelClass();

	bool Initialize(ID3D11Device*, char*, string);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4[], D3DXVECTOR4[], float, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float, D3DXVECTOR3);

	void SetScaling(float, float, float);
	void SetRotation(float, float, float);
	void SetPosition(float, float, float);

	int GetPolygonCount();
	void SetLightShaderClass(LightShaderClass*);

private:
	bool ReadObjFileCount(char*);
	bool LoadModel(char*);
	void ReleaseModel();

private:
	int m_vertexCount, m_indexCount, m_textureCount, m_normalCount, m_modelCount, m_faceCount;
	vector<vector<pair<int, int>>> m_eachModelFaceCount;

	vector<string> m_textureFileName;

	ModelClass::ModelType** m_model;

	ModelClass** m_models;
	LightShaderClass* m_lightShader;

	int polygoneCount;
};
#endif