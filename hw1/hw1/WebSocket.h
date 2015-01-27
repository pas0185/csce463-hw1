/*
* Patrick Sheehan
* CSCE463 HW1
* 27 January 2015
*
*/

#pragma once

#define INITIAL_BUF_SIZE 8000

#include "Headers.h"

class WebSocket {
	SOCKET sock;
	char* buf;
	int allocatedSize;
	int curPos;

	WebSocket();

	void DefaultSetup();
	bool Read();
	void Send(char* request);
};