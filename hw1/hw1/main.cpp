// hw1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void parseURLsFromFile(char* fileName);
char* dummyGETRequest();
void winsock_test(char* requestBuf);

int _tmain(int argc, _TCHAR* argv[])
{
	/*if (argc > 1) {
		parseURLsFromFile(argv[1]);
	}*/
	
	char req[] = "GET /courses/ HTTP/1.0\r\nHost: irl.cs.tamu.edu\r\nConnection: close\r\n\r\n";
	winsock_test(req);
	printf("Press enter key to continue");
	scanf("\n");
	return 0;
}

