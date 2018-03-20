#include "menu.h"

int Menu::MainMenu() {
	char input;
	while (1) {
		std::cout << "\n--------------------- MAIN MENU ---------------------\n"
				  << "-----------------------------------------------------\n"
				  << " | OPEN FILE (O) | PAST FROM BUFFER (P) | QUIT (Q) | \n\n"
				  << "your choice: ";
		std::cin >> input;
		if (input == 'O' || input == 'o') {
			return 1;
		}
		else if (input == 'P' || input == 'p') {
			return 2;
		}
		else if (input == 'Q' || input == 'q') {
			std::cout << "Goodbye!" << std::endl;
			return 0;
		}
		else {
			WrongInput(input);
		}
	}
}

int Menu::CleanMenu() {
	char input;
	while (1) {
		std::cout << "\n----------------------------------- CLEAN MENU ------------------------------------\n"
				  << " | CLEAN TABLE TAGS (T) | REMOVE HTML TAGS (H) | PRINT (P) | SAVE (S) | QUIT (Q) |\n\n"
				  << "your choice: ";
		std::cin >> input;
		if (input == 'T' || input == 't') {
			return 1;
		}
		else if (input == 'H' || input == 'h') {
			return 2;
		}
		else if (input == 'P' || input == 'p') {
			return 3;
		}
		else if (input == 'S' || input == 's') {
			return 4;
		}
		else if (input == 'Q' || input == 'q') {
			return 0;
		}
		else {
			WrongInput(input);
		}
	}
}