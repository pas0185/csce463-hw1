// hw1.cpp : Defines the entry point for the console application.
//

#include "Headers.h"

int HTMLParserTest();
char* parseURLString(char* url, char* hostnameOutput);
void parseURLsFromFile(char* fileName);
void winsock_test(char* requestBuf);
void htmlParserTest();

int _tmain(int argc, _TCHAR* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		char* url = argv[i], *request;
		printf("URL: %s\n", url);

		char* hostname = new char[strlen(url)];
		// parse url string into an HTTP GET request
		request = parseURLString(url, hostname);

		// create a web client to send our request
		WebSocket socket = WebSocket();
		socket.Setup(hostname);
		socket.Send(request);

		// and to get the resulting HTML file
		FILE *fp = fopen("test.html", "w+");
		socket.Read(fp);

		// parse the HTML file to count number of links

		printf("\n\n\n");
	}

	printf("Press enter key to continue");
	scanf("\n");
	return 0;
}
