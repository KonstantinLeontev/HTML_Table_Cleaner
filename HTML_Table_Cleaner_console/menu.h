#pragma once
#include <iostream>

class Menu {
public:
	Menu() {};
	~Menu() {};

	int MainMenu();
	int CleanMenu();
	bool PrintMenu();
	bool SaveMenu();

private:
	void WrongInput(const char &input) { std::cout << "Wrong input - '" << input << "'. Please, try again!" << std::endl; }
};