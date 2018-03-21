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
}
