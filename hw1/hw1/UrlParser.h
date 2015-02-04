/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*/

#pragma once

#include <iostream>

class URLParser 
{

public:
	// new in part-two
	static void parse(const char* url);

	// OLD
	char* buildGETRequest(char* host, char* port, char* request);
	char* parseURLString(char* url);
	char* getLastHostName();
};