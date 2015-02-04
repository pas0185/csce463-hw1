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

	unordered_map <string, DWORD> hostnameMap;
	const char* buildRequest(const char* type, const char* host, int port, const char* subrequest);
	void assignIPAddress(char* hostname, sockaddr_in* server);
	void performDNSLookup(char* hostname, sockaddr_in* server);

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