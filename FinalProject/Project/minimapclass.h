#pragma once

#ifndef _MINIMAPCLASS_H_
#define _MINIMAPCLASS_H_

#include "bitmapclass.h"
#include "textureshaderclass.h"

class MiniMapClass
{
public:
	MiniMapClass();
	MiniMapClass(const MiniMapClass&);
	~MiniMapClass();

	bool Initialize(ID3D11Device*, HWND, int, int, D3DXMATRIX, float, float, float, float);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, TextureShaderClass*);

	void PositionUpdate(float, float);

private:
	int m_mapLocationX ;
	int m_mapLocationY;
	int m_pointLocationX;
	int m_pointLocationY;
	float m_mapSizeX;
	float m_mapSizeY;
	float m_terrainXLeftBoundary;
	float m_terrainZLeftBoundary;
	float m_terrainXRightBoundary;
	float m_terrainZRightBoundary;
	D3DXMATRIX m_viewMatrix;
	BitmapClass *m_MiniMapBitmap;
	BitmapClass *m_Border;
	BitmapClass *m_Point;
};

#endif