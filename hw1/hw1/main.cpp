// hw1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int HTMLParserTest();
char* parseURLString(char* url);
void parseURLsFromFile(char* fileName);
void winsock_test(char* requestBuf);
void htmlParserTest();

int _tmain(int argc, _TCHAR* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		char* url = argv[i];
		printf("URL: %s\n", url);

		// parse url string into an HTTP GET request
		char* request = parseURLString(url);

		printf("GET request = %s", request);
		// create a web client to send our request and get resulting HTML file


		// parse the HTML file to count number of links


	}

	printf("Press enter key to continue");
	scanf("\n");
	return 0;
}
