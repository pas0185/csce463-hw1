/*
* Patrick Sheehan
* CSCE463 HW1
* 27 January 2015
* 
* References: 463 Sample Code by Dmitri Loguinov
*/

#include "WebSocket.h"

WebSocket::WebSocket()
{
	buf = new char[INITIAL_BUF_SIZE];
}

WebSocket::WebSocket(const char* hostname, int port, const char* subrequest)
{
	buf = new char[INITIAL_BUF_SIZE];

	Setup((char*)hostname, port);
}

void WebSocket::Setup(char* hostname, int port)
{
	WSADATA wsaData;

	// Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	// open a TCP socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("socket() generated error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	printStatusBeginning("Checking host uniqueness... ");

	in_addr IP;
	struct hostent *remote;

	// Check for a cached IP address
	std::map<string, in_addr>::const_iterator got = hostnameMap.find(hostname);
	if (got != hostnameMap.end()) {
		// Found cached IP matching the host
		IP = got->second;
		printf("found existing\n");
	}
	else {
		printf("passed\n");
		if ((remote = DNSLookup(hostname)) != NULL) {
			memcpy((char *)&(IP), remote->h_addr, remote->h_length);
		}
		else {
			printf("failed\n");
			return;
		}
	}

	// structure for connecting to server
	struct sockaddr_in server;

	server.sin_addr = IP;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	// Connect socket to server on correct port
	if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		printf("Connection error: %d\n", WSAGetLastError());
		return;
	}
}

void WebSocket::DownloadFunction(char* connectingOn, char* request)
{
	// My plan is to finish this function to make usage more universal

	clock_t start, end;
	int ms, byteCount, statusCode;

	// ******** CONNECTION ******** //
	printf("Connecting on %s... ", connectingOn);	// robots, page, etc
	start = clock();

	// TODO

	Send(request);

	end = clock();
	ms = msTime(start, end);
	printf("done in %d ms\n", ms);
	// ***************************** //

	// ******** DOWNLOADING ******** //
	printf("Loading... ");
	start = clock();

	// TODO

	end = clock();
	ms = msTime(start, end);
	printf("done in %d ms with %d bytes\n", ms, byteCount);
	// *************************** //

	// ******** VERIFYING ******** //
	printf("Verifying header... ");

	// TODO

	printf("status code %d", statusCode);
	// *************************** //

}

hostent* WebSocket::DNSLookup(char* hostname)
{
	// structure used in DNS lookups
	struct hostent *remote;
	clock_t start, end;
	in_addr IP;
	printStatusBeginning("Doing DNS... ");

	start = clock();	// timing DNS lookup
	if ((remote = gethostbyname(hostname)) != NULL) {
		memcpy((char *)&(IP), remote->h_addr, remote->h_length);
		end = clock();	// timing DNS lookup
		int total = 1000 * (end - start) / CLOCKS_PER_SEC;
		printf("done in %d ms, found %s\n", msTime(start, end), inet_ntoa(IP));
	}
	else {
		printf("failed\n");
	}
	
	return remote;
}

bool WebSocket::checkRobots(const char* hostname)
{
	clock_t start, end;
	const char* robotRequest = buildRequest("HEAD", hostname, "/robots.txt");
	char* buffer;

	printStatusBeginning("Connecting on robots...");
	start = clock();
	Send(robotRequest);
	end = clock();
	printf("done in %d ms\n", msTime(start, end));

	int status = ReadToBuffer(&buffer);

	if (status >= 400) {
		// Assume (only) a 4xx status is a green light to start crawling

		return true;
	}

	return false;
}

FILE* WebSocket::downloadPage(const char* hostname, const char* request)
{
	clock_t start, end;
	char* buffer;
	FILE *file;
	const char* pageRequest = buildRequest("GET", hostname, request);
	
	printStatusBeginning("Connecting on page... ", '*');
	start = clock();
	Send(pageRequest);
	end = clock();
	printf("done in %d ms\n", msTime(start, end));

	int status = ReadToBuffer(&buffer);
	if (200 <= status && status < 300) {
		// Successfully retrieved page
		file = fopen(hostname, "w+");
		fprintf(file, buffer);
		fclose(file);

		return file;
	}

	// TODO: explain detail reason
	printf("failed");
	return NULL;
}

void WebSocket::checkIPUniqueness(in_addr IP)
{
	printStatusBeginning("Checking IP uniqueness... ");
	printf("TODO\n");
}

void WebSocket::Send(const char* request)
{
	// send HTTP request
	if (send(sock, request, strlen(request), 0) == SOCKET_ERROR)
	{
		printf("Send error: %d\n", WSAGetLastError());
	}
}

int WebSocket::ReadToBuffer(char** buffer)
{
	// Receive data from socket and write it to the buffer 

	clock_t start, end;

	int bytesRead = 0, status = -1, num = 0;
	char *responseBuf;
	char *temp;

	printStatusBeginning("Loading... ");
	start = clock();	// timing loading file

	responseBuf = new char[INITIAL_BUF_SIZE];
	while ((num = recv(sock, responseBuf, INITIAL_BUF_SIZE, 0)) > 0)
	{
		bytesRead += num;

		// if need to resize buffer
		if (bytesRead + INITIAL_BUF_SIZE > strlen(responseBuf))
		{
			// Move old array to temp storage
			temp = new char[bytesRead];
			memcpy(temp, responseBuf, bytesRead);

			// Double size of buffer
			responseBuf = new char[2 * bytesRead];

			// Copy data over from temp
			memcpy(responseBuf, temp, bytesRead);
			
			// clear temp's memory
			memset(&temp[0], 0, sizeof(temp));
		}

		if (status < 0)
		{
			sscanf(responseBuf, "HTTP/1.0 %d \r\n", &status);
		}
	}

	// Truncate blank space
	responseBuf[bytesRead] = '\0';

	end = clock();	// timing for loading the file
	double total = (double)(end - start);
	printf("done in %d ms with %d bytes\n", (1000 * total / CLOCKS_PER_SEC), bytesRead);

	// Clean up resources
	memset(&responseBuf[0], 0, sizeof(responseBuf));

	printStatusBeginning("Verifying header... ");
	printf("status code %d\n", status);

	return status;
}

int WebSocket::ReadAndWriteToFile(char* filename)
{
	if (filename == NULL)
	{
		printf("An NULL file name was passed to Websocket for writing to");
		return -1;
	}

	clock_t start, end, total;

	// Write everything to the file, including header (TODO)
	FILE *fp = fopen(filename, "w+");
	int bytesRead = 0, status = -1, num = 0;
	char *responseBuf, *temp;

	// Receive data from socket
	printStatusBeginning("Loading... ");
	start = clock();	// timing loading file
	
	responseBuf = new char[INITIAL_BUF_SIZE];
	while ((num = recv(sock, responseBuf, INITIAL_BUF_SIZE, 0)) > 0)
	{
		bytesRead += num;

		// if need to resize buffer
		if (bytesRead + INITIAL_BUF_SIZE > strlen(responseBuf))
		{
			// Move old array to temp storage
			temp = new char[bytesRead];
			memcpy(temp, responseBuf, bytesRead);

			// Double size of buffer
			//responseBuf = new char[strlen(responseBuf) + INITIAL_BUF_SIZE];
			responseBuf = new char[2 * bytesRead];

			// Copy data over from temp
			memcpy(responseBuf, temp, bytesRead);

			// clear temp
			memset(&temp[0], 0, sizeof(temp));
		}

		//fprintf(fp, "%s", responseBuf);

		if (status < 0)
		{
			sscanf(responseBuf, "HTTP/1.0 %d \r\n", &status);
		}
	}

	// Truncate blank space
	responseBuf[bytesRead] = '\0';

	end = clock();	// timing for loading the file
	total = (double)(end - start);
	printf("done in %d ms with %d bytes\n", (1000 * total / CLOCKS_PER_SEC), bytesRead);

	// Write buffer to file
	fprintf(fp, "%s", responseBuf);
	fclose(fp);

	// Clean up resources
	memset(&responseBuf[0], 0, sizeof(responseBuf));

	printf("\t  Verifying header... ");
	printf("status code %d\n", status);

	return status;
}

// Helper Methods

void WebSocket::printStatusBeginning(const char* format, char special)
{
	printf("      %c %s", special, format);
}

void WebSocket::printStatusBeginning(const char* format)
{
	printf("        %s", format);
}

int WebSocket::msTime(clock_t start, clock_t end)
{
	double seconds = ((double)(end - start)) / CLOCKS_PER_SEC;
	return (int)(1000 * seconds);
}

const char* WebSocket::buildRequest(const char* type, const char* host, const char* subrequest)
{
	// Hostname is crucial
	if (host == NULL) {
		printf("Failed to create a GET request. Expected char* for hostname, received NULL");
		return NULL;
	}

	// Assign default value if no subrequest provided
	if (subrequest == NULL || subrequest == " ") {
		subrequest = "/";
	}

	// Build formatted request string
	int size = strlen(host) + strlen(subrequest) + strlen(useragent) + 50;
	char* FULLRequest = new char[size];
	sprintf(FULLRequest,
		"%s %s HTTP/1.0\r\n"
		"Host: %s\r\n"
		"User-agent: %s\r\n"
		"Connection: close\r\n\r\n",
		type, subrequest, host, useragent);

	return FULLRequest;
}