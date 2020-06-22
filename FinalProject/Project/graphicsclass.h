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
#include "multimodelclass.h"
#include "minimapclass.h"

#include "SkydomeClass.h"
#include "ShyDomeShaderClass.h"
#include "SkyPlaneClass.h"
#include "SkyPlaneShaderClass.h"

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 20000.0f;
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
	ModelClass* GetDrone();

	ModelClass* GetObj();
	void ReturnDrone();
	int GetObjType();
	void SetScreenSize(int, int);
	int GetWhatCar();
	void CarLightOn(int);
	void CarLightOff(int);

private:
	bool Render();
	
	void ObjCloseToDrone();

private:
	D3DClass* m_D3D;
	TextureShaderClass* m_TextureShader;
	CameraClass* m_Camera;
	LightShaderClass* m_LightShader;
	LightClass* m_DirectLight;
	LightClass* m_PointLight;
	TextClass* m_Text;

	ModelClass* drone;
	ModelClass* bottom;
	MultiModelClass* city;
	ModelClass* men[3];
	ModelClass* women[2];
	ModelClass* car[2];
	ModelClass* m_ChangeObject;
	vector<ModelClass*> allObj;
	int objTypeNum;

	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;
	SkyPlaneClass* m_SkyPlane;
	SkyPlaneShaderClass* m_SkyPlaneShader;

	D3DXVECTOR4* pointDiffuseColor;
	MiniMapClass* m_MiniMap;

	int m_screenWidth;
	int m_screenHeight;
	bool onCarLight1, onCarLight2;

public:
	int allPolygonCount;
};
#endif