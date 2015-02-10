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

	// MAP HOSTNAME STRINGS TO IP ADDRESSES
	std::map<std::string, in_addr> hostnameMap;

	// MAP IP ADDRESSES TO 

	int msTime(clock_t start, clock_t end);
	
public:
	void DownloadFunction(char* connectingOn, char* request);


	WebSocket();
	void Setup(char* hostname, int port, LPVOID pParam);	// Called by constructor
	in_addr getIPAddress(char* hostname);	// Checks for cached host->IP
	hostent* DNSLookup(char* hostname);		// Called if needed

	bool checkRobots(const char* hostname);
	void checkIPUniqueness(in_addr IP);
	FILE* downloadPage(const char* hostname, const char* request);
	const char* buildRequest(const char* type, const char* host, const char* subrequest);

	void Send(const char* request);
	int ReadToBuffer(char** buffer);

	int ReadAndWriteToFile(char* filename);

	void printStatusBeginning(const char* format);
	void printStatusBeginning(const char* format, char special);
};