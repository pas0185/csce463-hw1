// hw1.cpp : Defines the entry point for the console application.
//

#include "Headers.h"

int HTMLParserTest();
void parseURLsFromFile(char* fileName);
void winsock_test(char* requestBuf);
void htmlParserTest();

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		printf("No input file found in arguments. Please try again using the sytax:\n\thw1.exe <URL>\n");
		return 0;
	}
	
	int statusCode;
	char *url, *request, *hostname, *htmlFileName;
	UrlParser urlParser = UrlParser();
	WebSocket socket = WebSocket();
	HtmlParser htmlParser = HtmlParser();

	for (int i = 1; i < argc; i++)
	{
		url = new char[strlen(argv[i])];
		strcpy(url, argv[i]);
		printf("URL: %s\n", url);

		// parse url string into an HTTP GET request
		request = urlParser.parseURLString(url);
		hostname = urlParser.getLastHostName();

		// create a web client to send our request
		socket.Setup(hostname);
		socket.Send(request);

		// and to get the resulting HTML file
		htmlFileName = "test.html";
		statusCode = socket.ReadAndWriteToFile(htmlFileName);

		// TODO: just pass whole buffer instead of saving as a file first

		if (200 < statusCode && statusCode < 300)
		{
			// parse the HTML file to count number of links
			htmlParser.parse(htmlFileName, url);
		}

		printf("\n----------------------------\n");
		printf("HTTP/1.0 %d", statusCode);

		printf("\n\n\n");
	}

	printf("\n\nPress enter key to continue");
	scanf("\n");
	return 0;
}
