#include "cameraclass.h"
#include <math.h>

CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
	droneRotXZ = 0.0f;
	distDroneY = 0.0f;
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;

	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_preRotationX = m_rotationX;
	m_preRotationY = m_rotationY;
	m_preRotationZ = m_rotationZ;

	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;

	return;
}

void CameraClass::SetDroneRotate(D3DXVECTOR2 deltaMouse)
{
	droneRotXZ += deltaMouse.x / 2.0f;

	if (distDroneY >= -30.0f && distDroneY <= 30.0f)
	{
		distDroneY -= deltaMouse.y / 20.0f;
	}

	if (distDroneY > 27.0f)
		distDroneY = 27.0f;
	else if (distDroneY < -27.0f)
		distDroneY = -27.0f;

	if (droneRotXZ > 360.0f)
		droneRotXZ = 0.0f;
	else if (droneRotXZ < 0.0f)
		droneRotXZ = 360.0f;

}

void CameraClass::FollowPos(D3DXVECTOR3 pos, float objDist)
{
	float dist = objDist;

	dist = sqrt((dist*dist) - (distDroneY*distDroneY));

	float distX,distZ;
	float tempRotYZ;

	distX = cos((double)droneRotXZ*DegToRad);
	distZ = dist * sqrt(1 - (distX*distX));
	distX *= dist;

	m_positionX = pos.x + distX;
	m_positionY = pos.y + distDroneY;

	if(droneRotXZ > 180.0f)
		m_positionZ = pos.z - distZ;
	else
		m_positionZ = pos.z + distZ;
	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render(D3DXVECTOR3 lookAt)
{
	D3DXVECTOR3 up, position;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt -= position;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}