#pragma once

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float);

	CameraClass* GetCamera();		

private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	CameraClass* m_Camera;
	ModelClass* m_Model[4];
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	TextClass* m_Text;

public:
	bool useLightingEffect[3];
	int allPolygonCount;
};
#endif