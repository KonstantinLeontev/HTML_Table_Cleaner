#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <functional>

class Table {
public:
	Table() : m_Table(""){};
	~Table() {};

	bool ReadTableFromBuffer();
	bool ReadTableFromFile();
	void CleanTable();
	void SaveTable();
	void PrintTable();

private:
	void TagClean(const std::string &str);
	void TagRemove(const std::string &str);
	void EraseNoTable();
	std::string m_Table;
};