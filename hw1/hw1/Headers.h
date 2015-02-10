/*
* Patrick Sheehan
* CSCE463 HW1
* 27 January 2015
*
*/

#pragma once

#include <stdio.h>
#include <time.h>
#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>

#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <queue>
#include <map>
#include <set>
#include <string>

#include "URLParser.h"
#include "WebSocket.h"
#include "HTMLParserBase.h"
#include "HtmlParser.h"
#include "HtmlParser.h"
	
#define useragent "snailCrawlerPatrick/1.3"

class Parameters {
	// taken from 463-sample , main.cpp
	// this dedicated class is passed to all threads, acts as shared memory
public:
	HANDLE mutex;
	HANDLE finished;
	HANDLE eventQuit;
	std::queue<std::string> urlQueue;
	char* inputFile;
	clock_t clock;

	int numExtractedURLs;
	int numURLsWithUniqueHost;		// TODO
	int numSuccessfulDNSLookups;	// TODO
	int numURLsWithUniqueIP;		// TODO
	int numURLsPassedRobotCheck;
	int numCrawledURLs;
	int numLinks;
};
