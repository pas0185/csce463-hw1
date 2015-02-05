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

	int msTime(clock_t start, clock_t end);
	
public:
	void DownloadFunction(char* connectingOn, char* request);


	WebSocket();
	WebSocket(const char* hostname, int port, const char* subrequest);
	void Setup(char* hostname, int port);	// Called by constructor
	in_addr getIPAddress(char* hostname);	// Checks for cached host->IP
	in_addr DNSLookup(char* hostname);		// Called if needed

	bool checkRobots(const char* hostname);
	void checkIPUniqueness(in_addr IP);
	bool connectToPage(const char* hostname, const char* request);
	const char* buildRequest(const char* type, const char* host, const char* subrequest);

	void Send(const char* request);
	int ReadToBuffer(char** buffer);

	int ReadAndWriteToFile(char* filename);

	void printStatusBeginning(const char* format);
	void printStatusBeginning(const char* format, char special);
};