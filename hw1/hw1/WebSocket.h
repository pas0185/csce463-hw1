/*
* Patrick Sheehan
* CSCE463 HW1
* 27 January 2015
*
*/

#pragma once

#define INITIAL_BUF_SIZE 8000

#include "Headers.h"

class WebSocket 
{

private:
	SOCKET sock;
	char* buf;
	int allocatedSize;
	int curPos;
	int msTime(clock_t start, clock_t end);
	
public:
	WebSocket();
	void Setup(char* hostname, int port, LPVOID pParam);

	bool checkRobots(const char* hostname);
	FILE* downloadPage(const char* hostname, const char* request);
	const char* buildRequest(const char* type, const char* host, const char* subrequest);

	void Send(const char* request);
	int ReadToBuffer(char** buffer);
};