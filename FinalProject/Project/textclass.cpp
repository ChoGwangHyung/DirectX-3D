#include "textclass.h"

TextClass::TextClass()
{
	m_Font = 0;
	m_FontShader = 0;

	m_sentence1 = 0;
	m_sentence2 = 0;
	m_sentence3 = 0;
	m_sentence4 = 0;
	m_sentence5 = 0;
	m_sentence6 = 0;
	m_sentence7 = 0;
	m_sentence8 = 0;

	m_textColor[0] = 0;
	m_textColor[1] = 1;
	m_textColor[2] = 0;
}

TextClass::TextClass(const TextClass& other)
{
}

TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, "../Project/data/fontdata.txt", L"../Project/data/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_sentence1, 16, device);
	if (!result)
	{
		return false;
	}

	// Initialize the second sentence.
	result = InitializeSentence(&m_sentence2, 16, device);
	if (!result)
	{
		return false;
	}

	// Initialize the third sentence.
	result = InitializeSentence(&m_sentence3, 16, device);
	if (!result)
	{
		return false;
	}

	// Initialize the fourth sentence.
	result = InitializeSentence(&m_sentence4, 16, device);
	if (!result)
	{
		return false;
	}

	// Initialize the fifth sentence.
	result = InitializeSentence(&m_sentence5, 16, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_sentence6, 40, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_sentence7, 16, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_sentence8, 40, device);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	// Release the first sentence.
	ReleaseSentence(&m_sentence1);

	// Release the second sentence.
	ReleaseSentence(&m_sentence2);

	// Release the third sentence.
	ReleaseSentence(&m_sentence3);

	// Release the fourth sentence.
	ReleaseSentence(&m_sentence4);

	// Release the fifth sentence.
	ReleaseSentence(&m_sentence5);

	ReleaseSentence(&m_sentence6);

	ReleaseSentence(&m_sentence7);

	ReleaseSentence(&m_sentence8);

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX  orthoMatrix)
{
	bool result;

	// Draw the first sentence.
	result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Draw the second sentence.
	result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Draw the third sentence.
	result = RenderSentence(deviceContext, m_sentence3, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Draw the fourth sentence.
	result = RenderSentence(deviceContext, m_sentence4, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Draw the fifth sentence.
	result = RenderSentence(deviceContext, m_sentence5, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_sentence6, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_sentence7, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_sentence8, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;

	// Convert the cpu integer to string format.
	_itoa_s(cpu, tempString, 10);

	// Setup the cpu string.
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence2, cpuString, 20, 40, m_textColor[0], m_textColor[1], m_textColor[2], deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char fpsString[16];
	float red, green, blue;
	bool result;

	// Truncate the fps to below 10,000.
	if (fps > 9999)
	{
		fps = 9999;
	}

	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup the fps string.
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = m_textColor[0];
		green = m_textColor[1];
		blue = m_textColor[2];
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence1, fpsString, 20, 20, red, green, blue, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetPolygonNum(int polygonNum, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char polygonString[16];
	bool result;

	// Convert integer to string format.
	_itoa_s(polygonNum, tempString, 10);

	// Setup string.
	strcpy_s(polygonString, "Polygon: ");
	strcat_s(polygonString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence3, polygonString, 20, 60, m_textColor[0], m_textColor[1], m_textColor[2], deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetWorldTime(int time, ID3D11DeviceContext* deviceContext)
{
	char tempHourString[16];
	char tempMinuteString[16];
	char timeString[16];
	bool result;

	SetTextColor(time/60);

	// Convert integer to string format.
	_itoa_s(time / 60, tempHourString, 10);
	_itoa_s(time % 60, tempMinuteString, 10);

	// Setup string.
	strcpy_s(timeString, "Time: ");
	if (time / 60 < 10)
		strcat_s(timeString, "0");
	strcat_s(timeString, tempHourString);
	if (time % 60 < 10)
		strcat_s(timeString, ":0");
	else
		strcat_s(timeString, ":");
	strcat_s(timeString, tempMinuteString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence4, timeString, 700, 20, m_textColor[0], m_textColor[1], m_textColor[2], deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetDroneSpeed(float speed, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char speedString[16];
	bool result;

	// Convert integer to string format.
	sprintf_s(tempString, "%.2f", speed);
	
	// Setup string.
	strcpy_s(speedString, "Speed: ");
	strcat_s(speedString, tempString);
	strcat_s(speedString, "km/h");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence5, speedString, 700, 60, m_textColor[0], m_textColor[1], m_textColor[2], deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetDronePosition(float posX, float posY, float posZ, ID3D11DeviceContext* deviceContext)
{
	char xPosString[16];
	char yPosString[16];
	char zPosString[16];
	char posString[40];
	bool result;

	// Convert integer to string format.	
	sprintf_s(xPosString, "%.2f", posX);
	sprintf_s(yPosString, "%.2f", posY);
	sprintf_s(zPosString, "%.2f", posZ);

	// Setup string.
	strcpy_s(posString, "X: ");
	strcat_s(posString, xPosString);
	strcat_s(posString, " Y: ");
	strcat_s(posString, yPosString);
	strcat_s(posString, " Z: ");
	strcat_s(posString, zPosString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence6, posString, 20, 120, m_textColor[0], m_textColor[1], m_textColor[2], deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetObjectNum(int objectNum, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char objectString[16];
	bool result;

	// Convert integer to string format.
	_itoa_s(objectNum, tempString, 10);

	// Setup string.
	strcpy_s(objectString, "Object: ");
	strcat_s(objectString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence7, objectString, 20, 80, m_textColor[0], m_textColor[1], m_textColor[2], deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetScreenSize(int width, int height, ID3D11DeviceContext* deviceContext)
{
	char widthString[16];
	char heightString[16];
	char sizeString[40];
	bool result;

	// Convert integer to string format.	
	_itoa_s(width, widthString, 10);
	_itoa_s(height, heightString, 10);

	// Setup string.	
	strcpy_s(sizeString, "Screen Size : ");
	strcat_s(sizeString, widthString);
	strcat_s(sizeString, " X ");
	strcat_s(sizeString, heightString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence8, sizeString, 20, 100, m_textColor[0], m_textColor[1], m_textColor[2], deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextClass::SetViewMatrix(D3DXMATRIX viewMatrix)
{
	m_baseViewMatrix = viewMatrix;
}

bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for (i = 0; i < (*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the sentence vertex buffer.
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}

bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor);
	if (!result)
	{
		false;
	}

	return true;
}

void TextClass::SetTextColor(int time)
{
	//³·
	if (time < 19 && time > 5)
	{
		m_textColor[0] = 0;
		m_textColor[1] = 0;
		m_textColor[2] = 0;
	}
	//¹ã
	else
	{
		m_textColor[0] = 0;
		m_textColor[1] = 1;
		m_textColor[2] = 0;
	}
}
