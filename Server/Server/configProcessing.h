#pragma once
#include <fstream>

void comment(std::string& command, std::ifstream& file) {
	if (command == "#") { // Comment
		std::string input = "";
		while (input != "#")
			file >> input;
		file >> command;
	}
}