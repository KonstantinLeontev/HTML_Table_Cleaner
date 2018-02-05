#include "menu.h"

bool Menu::MainMenu() {
	char input;
	while (1) {
		std::cout << "\nWould you like to start (S) or quit (Q)?";
		std::cin >> input;
		if (input == 'S' || input == 's') {
			return true;
		}
		else if (input == 'Q' || input == 'q') {
			return false;
		}
		else {
			WrongInput();
		}
	}
}

bool Menu::ReadMenu() {
	char input;
	while (1) {
		std::cout << "\nWould you like to past table from buffer (B) or read from file (F)?";
		std::cin >> input;
		if (input == 'B' || input == 'b') {
			return true;
		}
		else if (input == 'F' || input == 'f') {
			return false;
		}
		else {
			WrongInput();
		}
	}
}

bool Menu::PrintMenu() {
	char input;
	while (1) {
		std::cout << "\nWould you like to print the table on the screen (P) or not (N)?";
		std::cin >> input;
		if (input == 'P' || input == 'p') {
			return true;
		}
		else if (input == 'N' || input == 'n') {
			return false;
		}
		else {
			WrongInput();
		}
	}
}

bool Menu::SaveMenu() {
	char input;
	while (1) {
		std::cout << "\nDo you want to save the table into a file (F) or not (N)?";
		std::cin >> input;
		if (input == 'F' || input == 'f') {
			return true;
		}
		else if (input == 'N' || input == 'n') {
			return false;
		}
		else {
			WrongInput();
		}

	}
}