#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_DirectLight = 0;
	m_PointLight = 0;
	m_TextureShader = 0;
	m_Text = 0;
	onCarLight1 = false;
	onCarLight2 = false;
	allPolygonCount = 0;

	m_SkyDome = 0;
	m_SkyDomeShader = 0;
	m_SkyPlane = 0;
	m_SkyPlaneShader = 0;

	//모델
	drone = 0;
	city = 0;
	for (int i = 0; i < 2; i++)
	{
		men[i] = 0;
		women[i] = 0;
		car[i] = 0;
	}
	men[2] = 0;

	m_ChangeObject = 0;
	allObj.clear();
	objTypeNum = 0;

	m_screenWidth = 800;
	m_screenHeight = 600;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -15.0f);
	m_Camera->Render(D3DXVECTOR3(0, 0, 1));
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// 미니맵 초기화
	m_MiniMap = new MiniMapClass;
	if (!m_MiniMap)
	{
		return false;
	}

	result = m_MiniMap->Initialize(m_D3D->GetDevice(), hwnd, screenWidth, screenHeight, baseViewMatrix, -1545, 1545, -1495, 1495);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the mini map object.", L"Error", MB_OK);
		return false;
	}

	//사람, 차 초기화
	for (int i = 0; i < 2; i++)
	{
		women[i] = new ModelClass;
		if (!women[i])
		{
			return false;
		}

		result = women[i]->Initialize(m_D3D->GetDevice(), "../Project/data/woman.obj", L"../Project/data/woman.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		women[i]->SetScaling(0.1, 0.1, 0.1);
		allPolygonCount += women[i]->GetPolygonCount();
		allObj.push_back(women[i]);

		car[i] = new ModelClass;
		if (!car[i])
		{
			return false;
		}

		result = car[i]->Initialize(m_D3D->GetDevice(), "../Project/data/car.obj", L"../Project/data/car.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		car[i]->SetScaling(20, 20, 20);
		allPolygonCount += car[i]->GetPolygonCount();
		allObj.push_back(car[i]);

		men[i] = new ModelClass;
		if (!men[i])
		{
			return false;
		}

		result = men[i]->Initialize(m_D3D->GetDevice(), "../Project/data/man.obj", L"../Project/data/man.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		men[i]->SetScaling(0.1, 0.1, 0.1);
		allPolygonCount += men[i]->GetPolygonCount();
		allObj.push_back(men[i]);
	}

	men[2] = new ModelClass;
	if (!men[2])
	{
		return false;
	}

	result = men[2]->Initialize(m_D3D->GetDevice(), "../Project/data/man.obj", L"../Project/data/man.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	men[2]->SetScaling(0.1, 0.1, 0.1);
	allPolygonCount += men[2]->GetPolygonCount();
	allObj.push_back(men[2]);

	women[0]->SetPosition(-258, -194, 402);
	women[1]->SetPosition(-276, -194, 738);
	car[0]->SetPosition(-513, -183, 585);
	car[1]->SetPosition(-474, -183, -206);
	men[0]->SetPosition(-293, -194, -953);
	men[1]->SetPosition(-307, -194, 998);
	men[2]->SetPosition(619, -194, -85);

	//드론 초기화
	drone = new ModelClass;
	if (!drone)
	{
		return false;
	}

	result = drone->Initialize(m_D3D->GetDevice(), "../Project/data/drone.obj", L"../Project/data/drone.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	drone->SetScaling(0.05, 0.05, 0.05);
	drone->SetPosition(0, 200, 0);
	allPolygonCount += drone->GetPolygonCount();
	m_ChangeObject = drone;

	//도시 초기화
	city = new MultiModelClass;
	if (!city)
	{
		return false;
	}

	result = city->Initialize(m_D3D->GetDevice(), "../Project/data/city.obj", "../Project/data/city texture/");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the city object.", L"Error", MB_OK);
		return false;
	}

	city->SetScaling(0.05, 0.05, 0.05);

	allPolygonCount += city->GetPolygonCount();

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_DirectLight = new LightClass;
	if (!m_DirectLight)
	{
		return false;
	}

	// Initialize the light object.
	m_DirectLight->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirectLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirectLight->SetDirection(0.0f, 0.0f, 1.0f);
	m_DirectLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirectLight->SetSpecularPower(32.0f);

	pointDiffuseColor = new D3DXVECTOR4[26];
	m_PointLight = new LightClass[26];
	if (!m_PointLight)
	{
		return false;
	}
	for (int i = 0; i < 26; i++)
	{
		m_PointLight[i].SetDiffuseColor(0, 0, 0, 1.0);
	}

	// Initialize the light object.

	for (int i = 11; i < 21; i++)
	{
		pointDiffuseColor[i] = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	pointDiffuseColor[0] = D3DXVECTOR4(1.0f, 0.6f, 0.3f, 1.0f);
	pointDiffuseColor[21]=D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	pointDiffuseColor[22]=D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	pointDiffuseColor[23]=D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	pointDiffuseColor[24]=D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	pointDiffuseColor[25]=D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	m_PointLight[0].SetPosition(-602.43, 410, 242.95);
	m_PointLight[1].SetPosition(-1043.82, -23, -694.10);
	pointDiffuseColor[1] = D3DXVECTOR4(0.6f, 0.6f, 1.0f, 1.0f);
	m_PointLight[2].SetPosition(-1155.9, -23, -582.71);
	pointDiffuseColor[2] = D3DXVECTOR4(0.6f, 0.6f, 1.0f, 1.0f);


	m_PointLight[3].SetPosition(-903.78, -193, -539.98);
	m_PointLight[4].SetPosition(-899.6, -193, -307.49);
	m_PointLight[5].SetPosition(-1005.41, -193, -423.38);
	m_PointLight[6].SetPosition(-775.34, -193, -423.61);
	m_PointLight[7].SetPosition(-856.31, -193, -450.7);
	m_PointLight[8].SetPosition(-862.83, -193, -389.01);
	m_PointLight[9].SetPosition(-918.45, -193, -386.28);
	m_PointLight[10].SetPosition(-913.07, -193, -463.29);

	pointDiffuseColor[3] = D3DXVECTOR4(0.76f, 0.48f, 1.0f, 1.0f);
	pointDiffuseColor[4] = D3DXVECTOR4(0.32f, 0.19f, 0.87f, 1.0f);
	pointDiffuseColor[5] = D3DXVECTOR4(0.84f, 1.0f, 0.14f, 1.0f);
	pointDiffuseColor[6] = D3DXVECTOR4(0.3f, 0.8f, 1.64f, 1.0f);
	pointDiffuseColor[7] = D3DXVECTOR4(1.0f, 0.9f, 0.37f, 1.0f);
	pointDiffuseColor[8] = D3DXVECTOR4(1.0f, 0.8f, 1.0f, 1.0f);
	pointDiffuseColor[9] = D3DXVECTOR4(0.98f, 0.48f, 0.3f, 1.0f);
	pointDiffuseColor[10] = D3DXVECTOR4(0.78f, 0.34f, 1.0f, 1.0f);

	m_PointLight[11].SetPosition(217.35, -193, -426.16);
	m_PointLight[12].SetPosition(365.56, -193, -423.43);

	m_PointLight[13].SetPosition(-380.60, -193, -456.74);
	m_PointLight[14].SetPosition(-335.85, -193, -504.07);
	m_PointLight[15].SetPosition(-266.2, -193, -504.36);
	m_PointLight[16].SetPosition(-219.37, -193, -459.40);
	m_PointLight[17].SetPosition(-219.41, -193, -390.25);
	m_PointLight[18].SetPosition(-263.89, -193, -345.18);
	m_PointLight[19].SetPosition(-332.45, -193, -343.63);
	m_PointLight[20].SetPosition(-378.21, -193, -390.24);


	m_PointLight[21].SetPosition(-306.82, -156, 1110.44);
	m_PointLight[22].SetPosition(0, 0, 0);
	m_PointLight[23].SetPosition(0, 0, 0);
	m_PointLight[24].SetPosition(0, 0, 0);
	m_PointLight[25].SetPosition(0, 0, 0);


	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_SkyDome = new SkyDomeClass;
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	m_SkyDomeShader = new SkyDomeShaderClass;
	result = m_SkyDomeShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}
	// 하늘 평면 객체를 생성합니다.
	m_SkyPlane = new SkyPlaneClass;
	if (!m_SkyPlane)
	{
		return false;
	}

	// 하늘 평면 객체를 초기화 합니다.
	result = m_SkyPlane->Initialize(m_D3D->GetDevice(), L"../Project/data/cloud001.dds", L"../Project/data/cloud002.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane object.", L"Error", MB_OK);
		return false;
	}
	// 하늘 평면 쉐이더 객체를 생성합니다.
	m_SkyPlaneShader = new SkyPlaneShaderClass;
	if (!m_SkyPlaneShader)
	{
		return false;
	}

	// 하늘 평면 쉐이더 객체를 초기화 합니다.
	result = m_SkyPlaneShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// 하늘 평면 쉐이더 객체를 해제합니다.
	if (m_SkyPlaneShader)
	{
		m_SkyPlaneShader->Shutdown();
		delete m_SkyPlaneShader;
		m_SkyPlaneShader = 0;
	}

	// 하늘 평면 객체를 해제합니다.
	if (m_SkyPlane)
	{
		m_SkyPlane->Shutdown();
		delete m_SkyPlane;
		m_SkyPlane = 0;
	}
	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the light object.
	if (m_DirectLight)
	{
		delete m_DirectLight;
		m_DirectLight = 0;
	}
	if (m_PointLight)
	{
		delete[] m_PointLight;
		m_PointLight = 0;
	}


	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (drone)
	{
		drone->Shutdown();
		delete drone;
		drone = 0;
	}

	if (city)
	{
		city->Shutdown();
		delete city;
		city = 0;
	}

	if (men)
	{
		for (int i = 0; i < 3; i++)
		{
			men[i]->Shutdown();
			delete men[i];
			men[i] = 0;
		}
	}

	if (women)
	{
		for (int i = 0; i < 2; i++)
		{
			women[i]->Shutdown();
			delete women[i];
			women[i] = 0;
		}
	}

	if (car)
	{
		for (int i = 0; i < 2; i++)
		{
			car[i]->Shutdown();
			delete car[i];
			car[i] = 0;
		}
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}
	return;
}

bool GraphicsClass::Frame(int fps, int cpu, float frameTime)
{
	bool result;
	static int i = 0;
	int time = m_SkyDome->GetTime();
	D3DXVECTOR3 dronePos = drone->GetPosition();

	if (time < 300 || time >= 1140) // 밤
	{
		m_SkyPlane->SetBrightness(0.25f);
		for (int i = 0; i < 22; i++)
		{
			m_PointLight[i].SetDiffuseColor(pointDiffuseColor[i]);
		}
	}
	else
	{
		m_SkyPlane->SetBrightness(0.65f);
		for (int i = 0; i < 22; i++)
		{
			m_PointLight[i].SetDiffuseColor(0, 0, 0, 1);
		}
	}

	m_DirectLight->DirectLightFrame(time);

	m_PointLight[0].SetPosition(dronePos.x, dronePos.y - 1.0f, dronePos.z);
	if (!drone->IsDroneMoved()) {
		D3DXVECTOR3 droneRotation = drone->GetRotation();
		if (i == 5)
			drone->SetRotation(0, droneRotation.y, 2.0f);
		else if (i == 10)
			drone->SetRotation(0, droneRotation.y, -2.0f);

		if (i > 10) i = 0;

		i++;
	}

	D3DXMATRIX carRotMat;
	D3DXVECTOR3 carRot = car[0]->GetRotation();
	D3DXVECTOR3 carLightPos1, carLightPos2,carPos;
	carLightPos1 = D3DXVECTOR3(12.76, 1.0, 36.89);
	carLightPos2 = D3DXVECTOR3(-12.76, 1.0, 36.89);
	carPos = car[0]->GetPosition();
	carRot = carRot * 0.0174532925f;
	D3DXMatrixIdentity(&carRotMat);
	D3DXMatrixRotationYawPitchRoll(&carRotMat, carRot.y, carRot.x, carRot.z);
	D3DXVec3TransformCoord(&carLightPos1, &carLightPos1, &carRotMat);
	D3DXVec3TransformCoord(&carLightPos2, &carLightPos2, &carRotMat);

	m_PointLight[22].SetPosition(carPos.x +carLightPos1.x , carPos.y + carLightPos1.y, carPos.z + carLightPos1.z);
	m_PointLight[23].SetPosition(carPos.x + carLightPos2.x, carPos.y + carLightPos2.y, carPos.z + carLightPos2.z);

	carLightPos1 = D3DXVECTOR3(12.76, 1.0, 36.89);
	carLightPos2 = D3DXVECTOR3(-12.76, 1.0, 36.89);
	carPos = car[1]->GetPosition();
	carRot = car[1]->GetRotation();
	carRot = carRot * 0.0174532925f;
	D3DXMatrixIdentity(&carRotMat);
	D3DXMatrixRotationYawPitchRoll(&carRotMat, carRot.y, carRot.x, carRot.z);
	D3DXVec3TransformCoord(&carLightPos1, &carLightPos1, &carRotMat);
	D3DXVec3TransformCoord(&carLightPos2, &carLightPos2, &carRotMat);

	m_PointLight[24].SetPosition(carPos.x + carLightPos1.x, carPos.y + carLightPos1.y, carPos.z + carLightPos1.z);
	m_PointLight[25].SetPosition(carPos.x + carLightPos2.x, carPos.y + carLightPos2.y, carPos.z + carLightPos2.z);

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	m_MiniMap->PositionUpdate(drone->GetPosition().x + 300, drone->GetPosition().z - 170);

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetPolygonNum(allPolygonCount, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetWorldTime(m_SkyDome->GetTime(), m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetDroneSpeed(drone->GetMoveSpeed(), m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	

	result = m_Text->SetDronePosition(dronePos.x, dronePos.y, dronePos.z, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetObjectNum(allObj.size() + 2, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetScreenSize(m_screenWidth, m_screenHeight, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}



	// 하늘 평면 프레임 처리를 수행합니다.
	m_SkyPlane->Frame();
	m_SkyPlane->Frame();
	m_SkyDome->Frame();
	m_DirectLight->SetAmbientColor(m_SkyDome->GetCenterColor() / 5);
	m_DirectLight->SetDiffuseColor(m_SkyDome->GetCenterColor());
	m_DirectLight->SetSpecularColor(m_SkyDome->GetCenterColor());

	return true;
}

CameraClass * GraphicsClass::GetCamera()
{
	return m_Camera;
}

ModelClass * GraphicsClass::GetDrone()
{
	return drone;
}

ModelClass* GraphicsClass::GetObj()
{
	ObjCloseToDrone();
	return m_ChangeObject;
}

void GraphicsClass::ReturnDrone()
{
	//빛끄기
	
	m_ChangeObject = drone;
	objTypeNum = 0;
}

int GraphicsClass::GetObjType()
{
		return objTypeNum;
}

void GraphicsClass::SetScreenSize(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
}

int GraphicsClass::GetWhatCar()
{
	if (objTypeNum == 1)
		return 1;
	if (objTypeNum == 4)
		return 2;

	return 0;
}

void GraphicsClass::CarLightOn(int key)
{
	if (key == 1)
	{
		if (onCarLight1 == false)
		{
			m_PointLight[22].SetDiffuseColor(pointDiffuseColor[22]);
			m_PointLight[23].SetDiffuseColor(pointDiffuseColor[23]);
			onCarLight1 = true;
		}
		else
		{
			CarLightOff(key);
		}
	}

	if (key == 2)
	{
		if (onCarLight2 == false) {
			m_PointLight[24].SetDiffuseColor(pointDiffuseColor[24]);
			m_PointLight[25].SetDiffuseColor(pointDiffuseColor[25]);
			onCarLight2 = true;
		}
		else
		{
			CarLightOff(key);
		}
	}
}

void GraphicsClass::CarLightOff(int key)
{
	if (key == 1)
	{
		m_PointLight[22].SetDiffuseColor(D3DXVECTOR4(0, 0, 0, 0));
		m_PointLight[23].SetDiffuseColor(D3DXVECTOR4(0, 0, 0, 0));
		onCarLight1 = false;
	}

	if (key == 2)
	{
		m_PointLight[24].SetDiffuseColor(D3DXVECTOR4(0, 0, 0, 0));
		m_PointLight[25].SetDiffuseColor(D3DXVECTOR4(0, 0, 0, 0));
		onCarLight2 = false;
	}
}

bool GraphicsClass::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix, skyDomeMatrix;
	bool result;
	D3DXVECTOR4 pointLightColor[26];
	D3DXVECTOR4 pointLightPosition[26];
	D3DXVECTOR3 att = { 1.0f,0.015f,0.0f };
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	for (int i = 0; i < 26; i++)
	{
		pointLightColor[i] = m_PointLight[i].GetDiffuseColor();
		pointLightPosition[i] = m_PointLight[i].GetPosition();
	}

	// Set of FillMode
	result = m_D3D->ChangeFillMode();
	if (!result)
	{
		return false;
	}

	// Generate the view matrix based on the camera's position.
	m_Camera->Render(m_ChangeObject->GetPosition());

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();
	D3DXVECTOR3 cameraPos = m_Camera->GetPosition();
	m_D3D->TurnOffCulling();
	D3DXMatrixTranslation(&skyDomeMatrix, cameraPos.x, cameraPos.y, cameraPos.z);
	m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), skyDomeMatrix, viewMatrix, projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());
	m_D3D->TurnOnCulling();

	// 구름이 sky dome color와 혼합되도록 첨가물 블렌딩을 가능하게합니다.
	m_D3D->EnableSecondBlendState();

	// 하늘 평면 쉐이더를 사용하여 하늘 평면을 렌더링합니다.
	m_SkyPlane->Render(m_D3D->GetDeviceContext());
	m_SkyPlaneShader->Render(m_D3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), skyDomeMatrix, viewMatrix, projectionMatrix, m_SkyPlane->GetCloudTexture1(), m_SkyPlane->GetCloudTexture2(), m_SkyPlane->GetTranslation(0), m_SkyPlane->GetTranslation(1), m_SkyPlane->GetTranslation(2), m_SkyPlane->GetTranslation(3), m_SkyPlane->GetBrightness());

	// 블렌드를 끕니다.
	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	//사람, 차
	for (int i = 0; i < 2; i++)
	{
		women[i]->Render(m_D3D->GetDeviceContext());

		result = m_LightShader->Render(m_D3D->GetDeviceContext(), women[i]->GetIndexCount(), worldMatrix*women[i]->GetWorldMatrix(), viewMatrix, projectionMatrix, women[i]->GetTexture(), pointLightColor, pointLightPosition, 50.0f, att, m_DirectLight->GetDiffuseColor(), m_DirectLight->GetAmbientColor(), m_Camera->GetPosition(), m_DirectLight->GetSpecularColor(), m_DirectLight->GetSpecularPower(), m_DirectLight->GetDirection());
		if (!result)
		{
			return false;
		}

		car[i]->Render(m_D3D->GetDeviceContext());

		result = m_LightShader->Render(m_D3D->GetDeviceContext(), car[i]->GetIndexCount(), worldMatrix*car[i]->GetWorldMatrix(), viewMatrix, projectionMatrix, car[i]->GetTexture(), pointLightColor, pointLightPosition, 50.0f, att, m_DirectLight->GetDiffuseColor(), m_DirectLight->GetAmbientColor(), m_Camera->GetPosition(), m_DirectLight->GetSpecularColor(), m_DirectLight->GetSpecularPower(), m_DirectLight->GetDirection());
		if (!result)
		{
			return false;
		}

		men[i]->Render(m_D3D->GetDeviceContext());

		result = m_LightShader->Render(m_D3D->GetDeviceContext(), men[i]->GetIndexCount(), worldMatrix*men[i]->GetWorldMatrix(), viewMatrix, projectionMatrix, men[i]->GetTexture(), pointLightColor, pointLightPosition, 50.0f, att, m_DirectLight->GetDiffuseColor(), m_DirectLight->GetAmbientColor(), m_Camera->GetPosition(), m_DirectLight->GetSpecularColor(), m_DirectLight->GetSpecularPower(), m_DirectLight->GetDirection());
		if (!result)
		{
			return false;
		}
	}

	men[2]->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), men[2]->GetIndexCount(), worldMatrix*men[2]->GetWorldMatrix(), viewMatrix, projectionMatrix, men[2]->GetTexture(), pointLightColor, pointLightPosition, 50.0f, att, m_DirectLight->GetDiffuseColor(), m_DirectLight->GetAmbientColor(), m_Camera->GetPosition(), m_DirectLight->GetSpecularColor(), m_DirectLight->GetSpecularPower(), m_DirectLight->GetDirection());
	if (!result)
	{
		return false;
	}

	//드론
	drone->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), drone->GetIndexCount(), worldMatrix*drone->GetWorldMatrix(), viewMatrix, projectionMatrix, drone->GetTexture(), pointLightColor, pointLightPosition, 30.0f, att, m_DirectLight->GetDiffuseColor(), m_DirectLight->GetAmbientColor(), m_Camera->GetPosition(), m_DirectLight->GetSpecularColor(), m_DirectLight->GetSpecularPower(), m_DirectLight->GetDirection());
	if (!result)
	{
		return false;
	}

	//도시
	city->SetLightShaderClass(m_LightShader);

	city->Render(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, pointLightColor, pointLightPosition,
		100.0f, att, m_DirectLight->GetDiffuseColor(), m_DirectLight->GetAmbientColor(), m_Camera->GetPosition(), m_DirectLight->GetSpecularColor(), m_DirectLight->GetSpecularPower(), m_DirectLight->GetDirection());

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// 미니맵을 렌더링 합니다.
	result = m_MiniMap->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix, m_TextureShader);
	if (!result)
	{
		return false;
	}

	// Turn on the alpha blending before rendering the text. 
	m_D3D->TurnOnAlphaBlending();
	//m_Text->SetViewMatrix(viewMatrix);
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::ObjCloseToDrone()
{
	if (m_ChangeObject != drone) return;

	float dist = 50.0f;
	int objIndex = -1;

	for (int i = 0; i < allObj.size(); i++)
	{
		if (dist >= D3DXVec3Length(&(drone->GetPosition() - allObj[i]->GetPosition())))
		{
			objIndex = i;
			break;
		}
	}

	if (objIndex != -1)
	{
		m_ChangeObject = allObj[objIndex];

		//차
		if (objIndex == 1 || objIndex == 4)
		{
			if (objIndex == 1)
			{
				objTypeNum = 1;
			}
			else
			{
				objTypeNum = 4;
			}			
		}
		//사람
		else objTypeNum = 2;
	}

	return;
}
