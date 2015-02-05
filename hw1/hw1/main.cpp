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
	else if (argc == 2)
	{
		// > hw1.exe <URL>

		// part 1 functionality
		int statusCode;
		char *url, *request, *hostname, *htmlFileName;
		URLParser urlParser = URLParser();
		WebSocket socket = WebSocket();
		HtmlParser htmlParser = HtmlParser();

		url = new char[strlen(argv[1])];
		strcpy(url, argv[1]);
		printf("URL: %s\n", url);

		// parse url string into an HTTP GET request
		request = urlParser.parseURLString(url);
		hostname = (char*)urlParser.getHostname(url);

		// create a web client to send our request
		socket.Setup(hostname, 80);
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
	else if (argc == 3)
	{
		// part 2 functionality

		// > hw1.exe <NUM-THREADS> <URL-INPUT.TXT>

		// First param
		istringstream in(argv[1]);
		int numThreads;
		if (in >> numThreads && in.eof())
		{
			cout << "Number of threads to spawn: " << numThreads << endl;
		}
		else
		{
			numThreads = 0;
			cout << "Error parsing number of threads" << endl;
		}

		// Second param
		string inputFile = argv[2];
		cout << "Provided input file: " << inputFile << endl;


		if (numThreads != 1)
		{
			cout << "Invalid input for number of threads to spawn" << endl;
			cout << "\tExpected: 1, but found " << numThreads << endl;
			cout << "Please provide input of the form:\n\t> hw1.exe <NUM-THREADS> <URL-INPUT.TXT>" << endl;
			return 0;
		}

		FileParser fileParser = FileParser();
		fileParser.parse(inputFile, numThreads);

	}

	printf("\n\nPress enter key to continue");
	scanf("\n");
	
	return 0;
}
