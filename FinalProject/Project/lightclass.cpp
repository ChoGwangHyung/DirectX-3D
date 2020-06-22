#include "LightClass.h"

LightClass::LightClass()
{
	diffuse = true;
	ambient = true;
	specular = true;
}
LightClass::LightClass(const LightClass& other)
{
}
LightClass::~LightClass()
{
}
void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}
void LightClass::SetAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor = D3DXVECTOR4(r, g, b, a);
}
void LightClass::SetSpecularColor(float r, float g, float b, float a)
{
	m_specularColor = D3DXVECTOR4(r, g, b, a);
}

void LightClass::SetDiffuseColor(D3DXVECTOR4 input)
{
	m_diffuseColor = input;
}
void LightClass::SetAmbientColor(D3DXVECTOR4 input)
{
	m_ambientColor = input;
}
void LightClass::SetSpecularColor(D3DXVECTOR4 input)
{
	m_specularColor = input;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
}
void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
}
void LightClass::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR4(x, y, z, 1.0f);
}

void LightClass::DirectLightFrame(int time)
{
	float delta = 1.0f / 360.0f;

	if (time>=360 && time < 1080) // ³·
	{
		m_direction.z -= delta;
		if (time < 720)
		{
			m_direction.y -= delta;
		}
		else
		{
			m_direction.y += delta;
		}
		
	}
	else
	{
		m_direction.z += delta;
		if(time>=1080)
			m_direction.y += delta;
		else if(time <360)
			m_direction.y -= delta;

	}

}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}
D3DXVECTOR4 LightClass::GetPosition()
{
	return m_position;
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

D3DXVECTOR4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}
