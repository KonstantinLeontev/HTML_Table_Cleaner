#pragma once
#include <iostream>

class Menu {
public:
	Menu() {};
	~Menu() {};

	bool MainMenu();
	bool ReadMenu();
	bool PrintMenu();
	bool SaveMenu();

private:
	void WrongInput() { std::cout << "Wrong input, try again!" << std::endl; }
};