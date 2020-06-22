#pragma once

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "inputclass.h"
#include "graphicsclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
#include "soundclass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	
	void SetEffectSound();
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	CameraClass* m_Camera;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	TimerClass* m_Timer;
	ModelClass* m_moveObj;
	SoundClass* m_Bgm;
	SoundClass* m_Effect;
	SoundClass* m_DroneSound;
	SoundClass* m_CarSound;
	SoundClass* m_PersonSound;

	int m_screenWidth, m_screenHeight;
};

// Function prototypes
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Globals
static SystemClass* ApplicationHandle = 0;
#endif

