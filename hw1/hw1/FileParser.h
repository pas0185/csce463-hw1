#pragma once

#include "Headers.h"

class FileParser
{
public:
	FileParser();
	~FileParser();

	static void parse(std::string inFile, int threadCount);
};

