/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*/

#pragma once

#include "Headers.h"


class UrlParser 
{
private:
	const char* useragent = "snailCrawlerPatrick/1.0";
	char hostname[256];

public:
	char* buildGETRequest(char* host, char* port, char* request);
	char* parseURLString(char* url);
	char* getLastHostName();
};