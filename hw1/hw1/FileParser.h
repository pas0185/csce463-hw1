#pragma once

#include "Headers.h"

class FileParser
{
public:
	FileParser();
	~FileParser();

	void parseFile(string inFile, int threadCount);

};

