#pragma once
#include "menu.h"
#include "table.h"

int main() {
	// Create UI object
	Menu menu;
	int choice(1);
	bool flag;

	// Main menu loop
	while (choice = menu.MainMenu()) {
		// Create a table object
		Table table;
		// Read a table
		switch (choice) {
		case 1: flag = table.ReadTableFromFile(); break;
		case 2: flag = table.ReadTableFromBuffer(); break;
		}
		// If not quit
		if (flag) {
			// Clean menu loop
			while (choice = menu.CleanMenu()) {
				switch (choice) {
				case 1: table.CleanTable(); break;
				case 2: table.CleanHTML(); break;
				case 3: table.PrintTable(); break;
				case 4: table.SaveTable(); break;
				}
			}
		}
		// Otherwise return to main menu
	}

	return 0;
}