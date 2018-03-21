#pragma once

// Reduce the size of the Win32 header file by exclude some of the less used API
#define WIN32_LEAN_AND_MEAN

// For ability to call win32 functions
#include <windows.h>

// Some custom classes for graphics and input
#include "inputclass.h"
#include "graphicsclass.h"

// Class for handling functions called in WinMain
class SystemClass {
public:
	SystemClass();
	SystemClass(const SystemClass &other);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};

// FUNCTION PROTOTYPES //
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GLOBALS //
static SystemClass* ApplicationHandle = 0;