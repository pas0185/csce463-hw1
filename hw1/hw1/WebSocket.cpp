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

	Setup((char*)hostname);

	// Use a HEAD request to get page statistics and check robots.txt
	const char* HEADRequest = buildRequest("HEAD", hostname, port, subrequest);
	Send(HEADRequest);
	ReadAndWriteToFile("HEADREQUEST-TEST.html");
	//ReadHEADResponse();
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
	sprintf(FULLRequest, "%s %s HTTP/1.0\r\nUser-agent: %s\r\nHost: %s\r\nConnection: close\r\n\r\n\0", type, subrequest, useragent, host);

	return FULLRequest;
}

//void WebSocket::performDNSLookup(char* hostname, struct sockaddr_in* server)
//{
//	// structure used in DNS lookups
//	struct hostent *remote;
//	clock_t start, end;
//
//	printf("\n\tDoing DNS... ");
//
//	start = clock();	// timing DNS lookup
//
//
//	// first assume that the string is an IP address
//	DWORD IP = inet_addr(hostname);
//	if (IP == INADDR_NONE)
//	{
//		// if not a valid IP, then do a DNS lookup
//		if ((remote = gethostbyname(hostname)) == NULL)
//		{
//			printf("Invalid string: neither FQDN, nor IP address\n");
//			return;
//		}
//		else // take the first IP address and copy into sin_addr
//			memcpy((char *)&(server->sin_addr), remote->h_addr, remote->h_length);
//	}
//	else
//	{
//		// if a valid IP, directly drop its binary version into sin_addr
//		server->sin_addr.S_un.S_addr = IP;
//	}
//
//	end = clock();	// timing DNS lookup
//	double total = (double)(end - start);
//	printf("done in %d ms, found %s", (1000 * total / CLOCKS_PER_SEC), inet_ntoa(server->sin_addr));
//}

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
		double total = 1000.0 * (end - start) / CLOCKS_PER_SEC;
		printf("done in %lf.2 ms, found %s", total, inet_ntoa(IP));
		return IP;
	}

	printf("failed");

}

in_addr WebSocket::getIPAddress(char* hostname)
{
	// Referenced CSCE 463 HW1p2 packet

	printf("\n\tChecking host uniqueness... ");
	
	in_addr IP;
	// Check for a cached IP address
	//map<string, in_addr>::const_iterator got = hostnameMap.find(hostname);
	//if (got == hostnameMap.end()) {
	if (true) {
		// IP has not been found yet
		printf("passed");
		IP = DNSLookup(hostname);
	}
	else {
		printf("failed (host isn't unique)");
		//IP = got->second;
	}

	return IP;
}

void WebSocket::Setup(char* hostname)
{
	WSADATA wsaData;
	clock_t start, end;

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


	// structure for connecting to server
	struct sockaddr_in server;
	server.sin_addr = getIPAddress(hostname);

	// setup the port # and protocol type
	server.sin_family = AF_INET;
	server.sin_port = htons(80);		// host-to-network flips the byte order


// ************************
	printf("\n\tChecking IP uniqueness...");
		printf("TODO");
	//checkUniqueneIP(server.sin_addr);


	printf("\n\tConnecting on robots...");
		printf("TODO");
	//connectOnRobots();

	
	printf("\n\tLoading...");
		printf("TODO");


	printf("\n\tVerifying header...");
	printf("TODO");



// ************************

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

void WebSocket::Send(const char* request)
{
	// send HTTP request
	if (send(sock, request, strlen(request), 0) == SOCKET_ERROR)
	{
		printf("Send error: %d\n", WSAGetLastError());
	}
}

void WebSocket::ReadHEADResponse()
{
	//int bytesRead = 0, status = -1, num = 0;
	//char *responseBuf, *temp;

	//// Receive data from socket
	//printf("\t  Loading... ");
	//start = clock();	// timing loading file

	//responseBuf = new char[INITIAL_BUF_SIZE];
	//while ((num = recv(sock, responseBuf, INITIAL_BUF_SIZE, 0)) > 0)
	//{
	//	bytesRead += num;

	//	// if need to resize buffer
	//	if (bytesRead + INITIAL_BUF_SIZE > strlen(responseBuf))
	//	{
	//		// Move old array to temp storage
	//		temp = new char[bytesRead];
	//		memcpy(temp, responseBuf, bytesRead);

	//		// Double size of buffer
	//		//responseBuf = new char[strlen(responseBuf) + INITIAL_BUF_SIZE];
	//		responseBuf = new char[2 * bytesRead];

	//		// Copy data over from temp
	//		memcpy(responseBuf, temp, bytesRead);
	//	}

	//	//fprintf(fp, "%s", responseBuf);

	//	if (status < 0)
	//	{
	//		sscanf(responseBuf, "HTTP/1.0 %d \r\n", &status);
	//	}
	//}

	//// Truncate blank space
	//responseBuf[bytesRead] = '\0';

	//end = clock();	// timing for loading the file
	//total = (double)(end - start);
	//printf("done in %d ms with %d bytes\n", (1000 * total / CLOCKS_PER_SEC), bytesRead);

	//// Write buffer to file
	//fprintf(fp, "%s", responseBuf);
	//fclose(fp);

	//// Clean up resources
	//memset(&responseBuf[0], 0, sizeof(responseBuf));
	//memset(&temp[0], 0, sizeof(temp));

	//printf("\t  Verifying header... ");
	//printf("status code %d\n", status);

	//return status;




}
void WebSocket::ReadGETResponse()
{
	// On success, request the html page and parse it
	bool hasValidBody = true;
	if (hasValidBody) {
		// Parse the page
		//HtmlParser::parse(...)
	}
	else {
		// TODO: explain why GET request failed
		cout << "Invalid page" << endl;
	}
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
	memset(&temp[0], 0, sizeof(temp));

	printf("\t  Verifying header... ");
	printf("status code %d\n", status);

	return status;
}
