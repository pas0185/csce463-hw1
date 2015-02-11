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
	int downloadPageAndCountLinks(const char* hostname, const char* request, const char* baseUrl, LPVOID pParam);
	const char* buildRequest(const char* type, const char* host, const char* subrequest);

	int Send(const char* request);
	int ReadToBuffer(int& status, char* buffer);
};