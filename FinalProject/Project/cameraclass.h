#pragma once

#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#define DegToRad 0.017453293f

#include <d3dx10math.h>

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetDroneRotate(D3DXVECTOR2 deltaMouse);
	void FollowPos(D3DXVECTOR3, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render(D3DXVECTOR3);
	void GetViewMatrix(D3DXMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float droneRotXZ, distDroneY;
	float m_preRotationX, m_preRotationY, m_preRotationZ;
	D3DXMATRIX m_viewMatrix;
};
#endif