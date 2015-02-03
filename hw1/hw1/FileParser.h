#pragma once

#include <iostream>

class FileParser
{
public:
	FileParser();
	~FileParser();

	static void parse(std::string inFile, int threadCount);
};

