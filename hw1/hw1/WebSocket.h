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

public:
	WebSocket();
	void Setup(char* hostname);
	int ReadAndWriteToFile(char* filename);
	void Send(char* request);
};