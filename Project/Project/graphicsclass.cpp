#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;

	for (int i = 0; i < 4; i++)
		m_Model[i] = 0;
	m_TextureShader = 0;
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

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -15.0f);

	for (int i = 0; i < 4; i++)
	{
		// Create the model object.
		m_Model[i] = new ModelClass;
		if (!m_Model[i])
		{
			return false;
		}

		// Initialize the model object.
		switch (i)
		{
		case 0: 
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Project/data/cube.obj", L"../Project/data/ground.dds");
			if (!result)
			{
				MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
				return false;
			}
			break;
		case 1:
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Project/data/car.obj", L"../Project/data/car.dds");
			if (!result)
			{
				MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
				return false;
			}
			break;
		case 2:
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Project/data/penguin.obj", L"../Project/data/penguin.dds");
			if (!result)
			{
				MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
				return false;
			}
			break;
		case 3:
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Project/data/chicken.obj", L"../Project/data/chicken.dds");
			if (!result)
			{
				MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
				return false;
			}
			break;
		}
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	for (int i = 0; i < 4; i++)
	{
		if (m_Model[i])
		{
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = 0;
		}
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

	return;
}

bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

CameraClass * GraphicsClass::GetCamera()
{
	return m_Camera;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Set of FillMode
	result = m_D3D->ChangeFillMode();
	if (!result)
	{
		return false;
	}

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.	
	// Set of FilteringMode
	result = m_TextureShader->ChangeFilteringMode(m_D3D->GetDevice());
	if (!result)
	{
		return false;
	}

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.		
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			m_Model[i]->SetScaling(100, 0.1, 100);			
			m_Model[i]->SetTranslation(0, -10, 50);
			break;
		case 1:						
			m_Model[i]->SetRotation('y', rotation);
			m_Model[i]->SetTranslation(-2, -2, 0);
			break;
		case 2:			
			m_Model[i]->SetScaling(2, 2, 2);
			m_Model[i]->SetRotation('y', rotation);
			break;
		case 3:						
			m_Model[i]->SetScaling(0.01, 0.01, 0.01);
			m_Model[i]->SetRotation('y', rotation);
			m_Model[i]->SetTranslation(4, -2, 0);
			break;
		}		

		m_Model[i]->Render(m_D3D->GetDeviceContext());

		// Render the model using the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(), m_Model[i]->GetWorldMatrix(), viewMatrix, projectionMatrix, m_Model[i]->GetTexture());
		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}