/*
* Patrick Sheehan
* CSCE463 HW1
* 5 February 2015
*
*/

#pragma once

#include "Headers.h"

class FileParser
{
public:
	FileParser();
	~FileParser();

	static void parse(std::string inFile, int threadCount);
};

