/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*
*/

#pragma once

#include "Headers.h"

class UrlParser 
{
private:
	char hostname[256];

public:
	char* buildGETRequest(char* host, char* port, char* request);
	char* parseURLString(char* url);
	char* getLastHostName();
};