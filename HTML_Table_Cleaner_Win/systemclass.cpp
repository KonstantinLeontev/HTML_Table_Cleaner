#include "systemclass.h"

SystemClass::SystemClass() : m_Input(0), m_Graphics(0){}

// Empty copy constructor because we don't need it in this project at all
// and don't need default constructor from some compilers 
SystemClass::SystemClass(const SystemClass& other){}

// Empty destructor because Shutdown() method will do this work.
// Certain windows functions don't call class destructors
// so it's good practice for windows programming
SystemClass::~SystemClass() {}

// This method does all setup for application
bool SystemClass::Initialize() {
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero before sending the variables
	// into the functions.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object. This object will be used to handle
	// reading the keyboard input from the user.
	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();

	// Create the graphics object. This object will handle rendering
	// all the graphics for the application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics) {
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result) {
		return false;
	}

	return true;
}

// All clean up - shuts down and releases everything associated with graphics and input object.
// As well shuts down the window and cleans up it's handles.
void SystemClass::Shutdown() {
	// Release the graphics object.
	if (m_Graphics) {
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if (m_Input) {
		delete m_Input;
		m_Input = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
}

// The main applicaton loop.
// The application processing is done in the Frame function wich is called each loop.
// 1. Check win system message.
// 2. Process system message.
// 3. Process app loop.
// 4. Check if user wanted to quit.
void SystemClass::Run() {
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from a window or a user.
	done = false;
	while (!done) {
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {
			// Otherwise do the frame processing.
			result = Frame();
			if (!result) {
				done = true;
			}
		}
	}
}

// All processing for the application is done here.
bool SystemClass::Frame()
{
	bool result;

	// Check if the user press escape and wants to exit the application.
	if (m_Input->IsKeyDown(VK_ESCAPE)) {
		return false;
	}

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

