/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*/

#pragma once

#include "Headers.h"

using namespace std;

class URLParser 
{
public:
	void parse(const char* url, LPVOID pParam);
	char* buildGETRequest(char* host, char* port, char* request);
	static const char* getHostname(const char* url);
	char* parseURLString(char* url);
	static const char* getSubrequest(const char* url);// , const char* hostname);
	static int getPort(const char* url);
};