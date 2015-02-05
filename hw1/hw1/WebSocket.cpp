/*
* Patrick Sheehan
* CSCE463 HW1
* 27 January 2015
* 
* References: 463 Sample Code by Dmitri Loguinov
*/

#include "WebSocket.h"


void DownloadFunction(char* connectOn)
{
	clock_t start, end;
	int ms, byteCount, statusCode;

	// ******** CONNECTION ******** //
	printf("Connecting on %s... ", connectOn);	// robots, page, etc
	start = clock();


	// TODO

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





WebSocket::WebSocket()
{
	buf = new char[INITIAL_BUF_SIZE];
}

WebSocket::WebSocket(const char* hostname, int port, const char* subrequest)
{
	buf = new char[INITIAL_BUF_SIZE];

	Setup((char*)hostname);

	// Use a HEAD request to get page statistics and check robots.txt
	/*const char* HEADRequest = buildRequest("HEAD", hostname, port, subrequest);
	Send(HEADRequest);
	ReadAndWriteToFile("HEADREQUEST-TEST.html");*/
	//ReadHEADResponse();
}

void WebSocket::Setup(char* hostname)
{
	WSADATA wsaData;
	clock_t start, end;
	int port = 80;

	//Initialize WinSock; once per program run
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

	in_addr IP = getIPAddress(hostname);

	// structure for connecting to server
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(80);


	// TODO:
	checkIPUniqueness(IP);


	bool crawlingAllowed = checkRobots(hostname, port);

	if (crawlingAllowed) {


		// GET request for page, then parse it
		printf("\n\tVerifying header...");
		printf("TODO");



		printf("\n\t* Connecting on page... ");
		start = clock();	// timing socket connection
		// connect to the server on port 80
		if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		{
			printf("Connection error: %d\n", WSAGetLastError());
			return;
		}
		end = clock();	// timing socket connection
		double total = (double)(end - start);
		printf("done in %d ms\n", (1000 * total / CLOCKS_PER_SEC));


	}
}
int msTime(clock_t start, clock_t end)
{
	double seconds = ((double)(end - start)) / CLOCKS_PER_SEC;
	return (int)(1000 * seconds);
}
in_addr WebSocket::getIPAddress(char* hostname)
{
	// Referenced CSCE 463 HW1p2 packet

	printf("\n\tChecking host uniqueness... ");

	in_addr IP;
	// Check for a cached IP address
	std::map<string, in_addr>::const_iterator got = hostnameMap.find(hostname);
	if (got == hostnameMap.end()) {
		// IP has not been found yet
		printf("passed (hostname is unique)");
		IP = DNSLookup(hostname);
	}
	else {
		printf("failed (hostname isn't unique)");
		IP = got->second;
	}

	return IP;
}

in_addr WebSocket::DNSLookup(char* hostname)
{
	// structure used in DNS lookups
	struct hostent *remote;
	clock_t start, end;
	in_addr IP;
	printf("\n\tDoing DNS... ");

	start = clock();	// timing DNS lookup
	if ((remote = gethostbyname(hostname)) != NULL) {
		memcpy((char *)&(IP), remote->h_addr, remote->h_length);
		end = clock();	// timing DNS lookup
		int total = 1000 * (end - start) / CLOCKS_PER_SEC;
		printf("done in %d ms, found %s", total, inet_ntoa(IP));
		return IP;
	}

	printf("failed");

}

bool WebSocket::checkRobots(char* hostname, int port)
{
	// use HEAD request
	// if the code is 4xx, you're good; unrestricted web crawling
	// anything else, FAILURE
	printf("\n\tConnecting on robots...");
	clock_t start, end;

	const char* request = buildRequest("HEAD", hostname, port, "/robots.txt");

	char* buffer;

	start = clock();
	Send(request);
	end = clock();
	double total = end - start;
	printf("done in %d ms\n", (1000 * total / CLOCKS_PER_SEC));

	int status = ReadToBuffer(&buffer);
	printf("\n\tRobots http status: %d\n", status);
	if (status >= 400) {
		// web crawling allowed for this host

		return true;
	}

	return true; // false
}

void WebSocket::checkIPUniqueness(in_addr IP)
{
	printf("\n\tChecking IP uniqueness...");
	printf("TODO");
}

const char* WebSocket::buildRequest(const char* type, const char* host, int port, const char* subrequest)
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
	clock_t start, end;

	int bytesRead = 0, status = -1, num = 0;
	char *responseBuf;
	char *temp;

	// Receive data from socket
	//printf("\t  Loading... ");
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

	printf("\t  Verifying header... ");
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
	printf("\t  Loading... ");
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
