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

	std::map<std::string, in_addr> hostnameMap;

	
public:
	WebSocket();
	WebSocket(const char* hostname, int port, const char* subrequest);
	void Setup(char* hostname);				// Called by constructor
	in_addr getIPAddress(char* hostname);	// Checks for cached host->IP
		in_addr DNSLookup(char* hostname);		// Called if needed

	bool checkRobots(char* hostname, int port);
		void checkIPUniqueness(in_addr IP);

	const char* buildRequest(const char* type, const char* host, int port, const char* subrequest);

	void Send(const char* request);
	int ReadToBuffer(char** buffer);

	int ReadAndWriteToFile(char* filename);
};