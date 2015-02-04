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

WebSocket::WebSocket(const char* hostname)
{
	buf = new char[INITIAL_BUF_SIZE];

	Setup((char*)hostname);
}

void WebSocket::Setup(char* hostname)
{
	WSADATA wsaData;

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

	// structure used in DNS lookups
	struct hostent *remote;

	// structure for connecting to server
	struct sockaddr_in server;

	printf("\t  Doing DNS... ");
	start = clock();	// timing DNS lookup

	// first assume that the string is an IP address
	DWORD IP = inet_addr(hostname);
	if (IP == INADDR_NONE)
	{
		// if not a valid IP, then do a DNS lookup
		if ((remote = gethostbyname(hostname)) == NULL)
		{
			printf("Invalid string: neither FQDN, nor IP address\n");
			return;
		}
		else // take the first IP address and copy into sin_addr
			memcpy((char *)&(server.sin_addr), remote->h_addr, remote->h_length);
	}
	else
	{
		// if a valid IP, directly drop its binary version into sin_addr
		server.sin_addr.S_un.S_addr = IP;
	}

	end = clock();	// timing DNS lookup
	total = (double)(end - start);
	printf("done in %d ms, found %s\n", (1000 * total / CLOCKS_PER_SEC), inet_ntoa(server.sin_addr));

	// setup the port # and protocol type
	server.sin_family = AF_INET;
	server.sin_port = htons(80);		// host-to-network flips the byte order


	printf("\t* Connecting on page... ");
	start = clock();	// timing socket connection
	// connect to the server on port 80
	if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		printf("Connection error: %d\n", WSAGetLastError());
		return;
	}
	end = clock();	// timing socket connection
	total = (double)(end - start);
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

void WebSocket::ReadHEADResponse();
void WebSocket::ReadGETResponse();
int WebSocket::ReadAndWriteToFile(char* filename)
{
	if (filename == NULL)
	{
		printf("An NULL file name was passed to Websocket for writing to");
		return -1;
	}

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
