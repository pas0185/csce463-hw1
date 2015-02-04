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

	//map<string, in_addr> hostnameMap;
	//unordered_map <string, in_addr> hostnameMap;
	const char* buildRequest(const char* type, const char* host, int port, const char* subrequest);
	in_addr getIPAddress(char* hostname);
	in_addr DNSLookup(char* hostname);

	

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