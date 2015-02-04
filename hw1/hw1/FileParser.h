#pragma once

#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>

class FileParser
{
public:
	FileParser();
	~FileParser();

	static void parse(std::string inFile, int threadCount);
};

