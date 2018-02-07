#pragma once
#include "table.h"

bool Table::ReadTableFromBuffer() {
	while (1) {
		std::cout << "\nPast table from buffer here or press 'Q' to quit or 'S' to stop input: ";
		// first getline eats a newline character from menu
		std::getline(std::cin, m_Table);
		std::string temp;
		while (std::getline(std::cin, temp) && temp != "s" && temp != "S" && temp != "q" && temp != "Q") {
			m_Table.append(temp);
			m_Table.push_back('\n');
		}
		// If loop was stopped by 'S'
		if (temp == "S" || temp == "s") {
			if (m_Table.find("<table") != std::string::npos) {
				std::cout << "Reading is complete!" << std::endl;
				return true;
			}
			else {
				std::cout << "There's no any tables here. Please try again!" << std::endl;
				// Clear all previous 
				m_Table.clear();
			}
		}
		// Otherwise it was quit command 'Q'
		else {
			std::cout << "Reading was cancelled!" << std::endl;
			// Return to main menu
			// There is no need to clear input because of object deleting in main loop 
			return false;
		}
		
	}
}

bool Table::ReadTableFromFile() {
	std::ifstream inputFile;
	std::string fileName;
	// Open file dialog loop
	while (1) {
		// Input a file name
		std::cout << "\nInput a file name to open or 'Q' to quit: ";
		std::cin >> fileName;
		// If it's not a quit command
		if (fileName != "Q" && fileName != "q") {
			// Open the file with name "fileName"
			inputFile.open(fileName, std::ios::in);
			// If it was successful opening, proceed with output
			if (inputFile.is_open()) {
				std::string tempStr;
				// read line by line
				while (getline(inputFile, tempStr)) {
					m_Table += tempStr;
					m_Table.push_back('\n');
				}
				std::cout << "Reading is complete!" << std::endl;
				// Close the file
				inputFile.close();
				return true;
			}
			// Otherwise print an error message
			else {
				std::cout << "Can't open a file! Try again or quit." << std::endl;
			}
		}
		// Otherwise terminate reading method
		else {
			std::cout << "Reading was cancelled!" << std::endl;
			return false;
		}
	}
}

void Table::TagRemove(const std::string &str) {
	int openTagSize, closeTagSize, cnt(0);
	openTagSize = str.size();
	closeTagSize = 1 + openTagSize;
	// Copy open tag for making close tag from it
	std::string closeTag = str;
	
	// Get an interator to begin of tag
	std::string::iterator itr = closeTag.begin();
	// Increase it to point to second charachter
	itr++;
	// Add '/' to complete the close tag
	closeTag.insert(itr, '/');
	
	// Iterate through the table and erase open tags
	size_t pos, pos2;
	while ((pos = m_Table.find(str)) != std::string::npos) {
		// Looking for close bracket from pos
		pos2 = m_Table.find('>', pos);
		// If there is one
		if (pos2 != std::string::npos) {
			// Erase all from '<' to '>'
			m_Table.erase(pos, (pos2 - pos + 1));
			// Increase counter for message
			cnt++;
		}
		// Otherwise there is no close bracket for this tag
		else {
			// So just clear the tag name only
			m_Table.erase(pos, openTagSize);
			// And show error message
			std::cout << "Error! There is no '>' for " << str << ">!" << std::endl;
		}
	}
	std::cout << cnt << ' ' << str << "> tags was removed!" << std::endl;
	// Erase close tags
	cnt = 0;
	while ((pos = m_Table.find(closeTag)) != std::string::npos) {
		m_Table.erase(pos, closeTagSize + 1);
		cnt++;
	}
	std::cout << cnt << ' ' << closeTag << "> tags was removed!" << std::endl;
}

void Table::TagClean(const std::string &str) {
	int num = str.size();
	int cnt(0);
	std::string::iterator curr, first, last, end;
	
	// Iterate trough the table and clean all "str" up
	for (curr = m_Table.begin(); curr != m_Table.end();) {
		end = m_Table.end();
		// Get the first pointer to tag content
		first = search(curr, end, str.begin(), str.end());
		// Check if the first iterator don't point to the end
		if (first != end) {
			// Add the size of the tag to point after the tag name
			first += num;
			// get the second pointer to close tag
			last = find(first, end, '>');
			if (last != end) {
				// erase all between first and last and get iterator to this place
				curr = m_Table.erase(first, last);
				// Increase counter of cleaned tags
				cnt++;
			}
			else {
				// Show error message
				std::cout << "ERROR. Cannot find the close bracket '>' for " << str << "> tag! Check your table for correct tags." << std::endl;
				// Skip this place and continue
				curr = first;
			}
		}
		else {
			curr = end;
		}
	}
	// Output the number of cleaned tags
	std::cout << cnt << ' ' << str << "> tags has been cleaned up!" << std::endl;
}

void Table::EraseNoTable() {
	std::string::iterator curr, first, last;
	std::string openTag = "<table", closeTag = "</table>";
	// search for <table> tag
	first = m_Table.begin();
	last = std::search(first, m_Table.end(), openTag.begin(), openTag.end());
	// Clear all the code before <table> tag
	curr = m_Table.erase(first, last);
	
	// Clear between many <table> tags if there are any of them
	while (curr != m_Table.end()) {
		last = m_Table.end();
		// Search for close </table> tag
		first = std::search(curr, last, closeTag.begin(), closeTag.end());
		// If there is close tag
		if (first != last) {
			// Move iterator to first character after </table>
			first += 8;
			// If it's not the end of string
			if (first < last) {
				// Search for next table if there are any
				last = std::search(first, last, openTag.begin(), openTag.end());
				// Erase this part of string between tags or from last tag to the end if last == end
				curr = m_Table.erase(first, last);
			}
			// Otherwise it's the end of the string, so point curr to the end for loop stop
			else {
				curr = last;
			}
		}
		// Otherwise show error message
		else {
			std::cout << "There is no </table> tag!" << std::endl;
			curr = last;
		}	
	}
	// Complete function message
	std::cout << "All staff beyond the table(s) was erased!" << std::endl;
}

void Table::CleanTable() {
	// Add a line before output
	std::cout << '\n';
	// Clear all staff befor and after the table
	EraseNoTable();

	// All tags for cleaning
	std::string tagForClean[] = { "<table", "<tr", "<th", "<td", "<caption" };
	// All tags for deleting
	std::string tagForRemove[] = { "<colgroup", "<col", "<thead", "<tbody", "<tfoot" };

	for (int i = 0; i < 5; i++) {
		TagClean(tagForClean[i]);
		TagRemove(tagForRemove[i]);
	}
}

void Table::CleanHTML() {
	std::string tags[] = { "<p", "<div", "<h" };

	std::cout << '\n';
	for (int i = 0; i < 3; i++) {
		TagRemove(tags[i]);
	}
}

void Table::PrintTable() {
	std::cout << "\n--------- NEW TABLE ---------\n\n";
	std::cout << m_Table << std::endl;
}

void Table::SaveTable() {
	std::ofstream outputFile;
	std::string fileName;

	std::cout << "\nThe name of the file to output: ";
	std::cin >> fileName;
	// Open or create a file
	outputFile.open(fileName, std::ios::in | std::ios::trunc);
	// If file was open
	if (outputFile.is_open()) {
		outputFile << m_Table;
		outputFile.close();
		std::cout << "Writing to file complete!" << std::endl;
	}
	// Otherwise show the error message
	else {
		std::cout << "\nCannot open the file!" << std::endl;
	}
}