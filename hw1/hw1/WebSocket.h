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
	clock_t start, end, total;
	const char* buildRequest(const char* type, const char* host, int port, const char* subrequest);

public:
	WebSocket();
	WebSocket(const char* hostname, int port, const char* subrequest);
	void Setup(char* hostname);
	int ReadToBuffer(char** buffer);

	int ReadAndWriteToFile(char* filename);

	void ReadHEADResponse();
	void ReadGETResponse();
	void Send(const char* request);
};