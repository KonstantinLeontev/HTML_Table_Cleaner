#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "menu.h"
#include "table.h"

int main() {
	// Create UI object
	Menu menu;
	bool flag;

	// Main loop
	while (menu.MainMenu()) {
		// Create a table object
		Table table;
		// Read a table
		if (menu.ReadMenu()) {
			flag = table.ReadTableFromBuffer();
		}
		else {
			flag = table.ReadTableFromFile();
		}
		if (flag) {
			// Clean the table up
			table.CleanTable();
			// Output the result table on the screen
			if (menu.PrintMenu()) {
				table.PrintTable();
			}
			// Save the table out
			if (menu.SaveMenu()) {
				table.SaveTable();
			}
		}
	}

	return 0;
}