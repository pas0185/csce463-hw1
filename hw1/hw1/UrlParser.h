/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*
*/

#pragma once

#include "Headers.h"

class UrlParser {

public:
	char hostname[256];
	char* parseURLString(char* url);
	char* getLastHostName();
};