/*
* Patrick Sheehan
* CSCE463 HW1
* 27 January 2015
*
*/

#include <stdio.h>
#include <winsock2.h>

#define INITIAL_BUF_SIZE 8000

// Taken from homework handout
class Socket {
	SOCKET sock;
	char* buf;
	int allocatedSize;
	int curPos;

	Socket();
	bool Read();
	void Send(char* request);
};