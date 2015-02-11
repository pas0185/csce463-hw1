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
void WebSocket::Setup(char* hostname, int port, LPVOID pParam)
{
	WSADATA wsaData;

	bool uniqueHost = false;
	bool successfulDNS = false;
	bool uniqueIP = false;

	Parameters *p = ((Parameters*)pParam);

	// Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		// WSA failure
		WSACleanup();
		return;
	}

	// open a TCP socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		WSACleanup();
		return;
	}

	struct in_addr IP;
	std::map<std::string, in_addr>::const_iterator cachedHost;

	WaitForSingleObject(p->mutex, INFINITE);	// lock mutex

	size_t prevHostSetSize = p->visitedHostSet.size();
	size_t prevIPSetSize = p->visitedIPSet.size();

	p->visitedHostSet.insert(hostname);
	if (p->visitedHostSet.size() > prevHostSetSize) {
		// unique host
		(p->numURLsWithUniqueHost) += 1;

		// DNS lookup
		struct hostent *remote = gethostbyname(hostname);
		if (remote != NULL) {
			memcpy(&IP, remote->h_addr_list[0], sizeof(struct in_addr));
			//memcpy((char *)&(IP), remote->h_addr, remote->h_length);

			p->visitedIPSet.insert(IP);
			if (p->visitedIPSet.size() > prevIPSetSize) {
				// unique IP
				(p->numURLsWithUniqueIP) += 1;
			}
		}
	}
	ReleaseMutex(p->mutex);						// unlock mutex

	// structure for connecting to server
	struct sockaddr_in server;
	server.sin_addr = IP;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	// Connect socket to server on correct port
	if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
		return;
	}
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
	//printf("done in %d ms\n", msTime(start, end));

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
	//printf("done in %d ms\n", msTime(start, end));

	int status = ReadToBuffer(&buffer);
	if (200 <= status && status < 300) {
		// Successfully retrieved page
		file = fopen(hostname, "w+");
		fprintf(file, buffer);
		fclose(file);

		return file;
	}

	// TODO: explain detail reason
	//printf("failed");
	return NULL;
}

void WebSocket::checkIPUniqueness(in_addr IP)
{
	printStatusBeginning("Checking IP uniqueness... ");
	//printf("TODO\n");
}

void WebSocket::Send(const char* request)
{
	// send HTTP request
	if (send(sock, request, strlen(request), 0) == SOCKET_ERROR)
	{
		//printf("Send error: %d\n", WSAGetLastError());
	}
}

int WebSocket::ReadToBuffer(char** buffer)
{
	// Receive data from socket and write it to the buffer 

	//clock_t start, end;

	int bytesRead = 0, status = -1, num = 0;
	char *responseBuf;
	char *temp;

	//printStatusBeginning("Loading... ");
	//start = clock();	// timing loading file

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
			sscanf(responseBuf, "HTTP/1.0 %d", &status);
		}
	}

	// Truncate blank space
	responseBuf[bytesRead] = '\0';

	//end = clock();	// timing for loading the file
	//double total = (double)(end - start);
	//printf("done in %d ms with %d bytes\n", (1000 * total / CLOCKS_PER_SEC), bytesRead);

	// Clean up resources
	memset(&responseBuf[0], 0, sizeof(responseBuf));

	//printStatusBeginning("Verifying header... ");
	//printf("status code %d\n", status);

	return status;
}

// Helper Methods

void WebSocket::printStatusBeginning(const char* format, char special)
{
	//printf("      %c %s", special, format);
}

void WebSocket::printStatusBeginning(const char* format)
{
	//printf("        %s", format);
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
		//printf("Failed to create a GET request. Expected char* for hostname, received NULL");
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