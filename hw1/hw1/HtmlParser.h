/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*
*/

#pragma once

#include "Headers.h"

class HtmlParser
{
public:
	int parse(FILE* file, char* baseUrl);

	void parse(char* filename, char* baseUrl);
};