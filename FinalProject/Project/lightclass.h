#pragma once

#include <d3dx10math.h>

class LightClass
{
public:
	bool diffuse;
	bool ambient;
	bool specular;

	LightClass();
	LightClass(const LightClass&);
	~LightClass();
	void SetDiffuseColor(float, float, float, float);
	void SetAmbientColor(float, float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetDiffuseColor(D3DXVECTOR4);
	void SetAmbientColor(D3DXVECTOR4);
	void SetSpecularColor(D3DXVECTOR4);
	void SetDirection(float, float, float);
	void SetSpecularPower(float);
	void SetPosition(float, float, float);
	void DirectLightFrame(int);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR4 GetPosition();
	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetSpecularColor();
	D3DXVECTOR3 GetDirection();
	float GetSpecularPower();



private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR4 m_specularColor;
	D3DXVECTOR3 m_direction;
	float m_specularPower;
	D3DXVECTOR4 m_position;


};

