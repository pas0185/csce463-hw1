/*
* Patrick Sheehan
* CSCE463 HW1
* 27 January 2015
* 
*/

#include "WebSocket.h"
#include <time.h>

WebSocket::WebSocket()
{
	buf = new char[INITIAL_BUF_SIZE];
}

void WebSocket::Send(char* request)
{
	// send HTTP requests here
	if (send(sock, request, strlen(request), 0) == SOCKET_ERROR)
	{
		printf("Send error: %d\n", WSAGetLastError());
	}
}

void WebSocket::Setup(char* hostname)
{
	// Below taken from 463 Sample Code - by Dmitri Loguinov

	WSADATA wsaData;
	clock_t start, end, total;

	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	// open a TCP socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

	printf("\t  Doing DNS...");

	start = clock();

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

	end = clock();
	total = (double)(end - start);
	printf("done in %d ms, found %s\n", (1000 * total / CLOCKS_PER_SEC), inet_ntoa(server.sin_addr));

	// setup the port # and protocol type
	server.sin_family = AF_INET;
	server.sin_port = htons(80);		// host-to-network flips the byte order


	printf("\t* Connecting on page...");
	start = clock();
	// connect to the server on port 80
	if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		printf("Connection error: %d\n", WSAGetLastError());
		return;
	}
	end = clock();
	total = (double)(end - start);
	printf("done in %d ms\n", (1000 * total / CLOCKS_PER_SEC));
}

void WebSocket::Read(FILE* fp)
{
	if (fp == NULL)
	{
		printf("NULL File pointer pass to Read()");
	}

	// receive data here
	char responseBuf[INITIAL_BUF_SIZE];
	while (recv(sock, responseBuf, INITIAL_BUF_SIZE, 0) > 0)
	{
		fprintf(fp, "%s", responseBuf);
	}
}
