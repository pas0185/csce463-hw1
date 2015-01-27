/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*
*/

#include <stdio.h>
#include <winsock2.h>

#define RESPONSE_BUFFER_LENGTH 1000

void winsock_test(char* requestBuf)
{
	// string pointing to an HTTP server (DNS name or IP)
	char str[] = "www.tamu.edu";
	//char str [] = "128.194.135.72";

	WSADATA wsaData;

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

	// first assume that the string is an IP address
	DWORD IP = inet_addr(str);
	if (IP == INADDR_NONE)
	{
		// if not a valid IP, then do a DNS lookup
		if ((remote = gethostbyname(str)) == NULL)
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

	// setup the port # and protocol type
	server.sin_family = AF_INET;
	server.sin_port = htons(80);		// host-to-network flips the byte order

	// connect to the server on port 80
	if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		printf("Connection error: %d\n", WSAGetLastError());
		return;
	}

	printf("Successfully connected to %s (%s) on port %d\n", str, inet_ntoa(server.sin_addr), htons(server.sin_port));

	// send HTTP requests here
	if (send(sock, requestBuf, strlen(requestBuf), 0) == SOCKET_ERROR)
	{
		printf("Send error: %d\n", WSAGetLastError());
	}

	// receive data here
	char responseBuf[RESPONSE_BUFFER_LENGTH];
	while (recv(sock, responseBuf, RESPONSE_BUFFER_LENGTH, 0) > 0)
	{
		printf("%s", responseBuf);
	}

	// close the socket to this server; open again for the next one
	closesocket(sock);

	// call cleanup when done with everything and ready to exit program
	WSACleanup();
}

//int socketTest()
//{	// Guidelines and sample code based off of lecture slides at:
//	// http://irl.cse.tamu.edu/courses/463/1-22-15.pdf
//	
//
//	DWORD dwError;
//	char **pAlias;
//	int i = 0;
//	struct in_addr addr;
//
//	// WILL BE PARAMETERS
//	char ipAddrNumbers[] = "128.194.135.72";
//	char ipAddrName[] = "www.tamu.edu";
//	// ******************
//
//
//
//	// Open a socket
//	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (sock == INVALID_SOCKET)
//	{
//		printf("socket() error %d\n", WSAGetLastError());
//		WSACleanup();
//		return 1;
//	}
//
//	// Get IP address of server in URL
//	long address;
//	if ((address = inet_addr(ipAddrNumbers)) == INADDR_NONE)
//	{
//		struct hostent* remoteHost = gethostbyname(ipAddrName); 
//		
//		// From MSDN:
//		if (remoteHost == NULL) {
//			dwError = WSAGetLastError();
//			if (dwError != 0) {
//				if (dwError == WSAHOST_NOT_FOUND) {
//					printf("Host not found\n");
//					return 1;
//				}
//				else if (dwError == WSANO_DATA) {
//					printf("No data record found\n");
//					return 1;
//				}
//				else {
//					printf("Function failed with error: %ld\n", dwError);
//					return 1;
//				}
//			}
//		}
//		else {
//			printf("Function returned:\n");
//			printf("\tOfficial name: %s\n", remoteHost->h_name);
//			for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++) {
//				printf("\tAlternate name #%d: %s\n", ++i, *pAlias);
//			}
//			printf("\tAddress type: ");
//			switch (remoteHost->h_addrtype) {
//			case AF_INET:
//				printf("AF_INET\n");
//				break;
//			case AF_NETBIOS:
//				printf("AF_NETBIOS\n");
//				break;
//			default:
//				printf(" %d\n", remoteHost->h_addrtype);
//				break;
//			}
//			printf("\tAddress length: %d\n", remoteHost->h_length);
//
//			i = 0;
//			if (remoteHost->h_addrtype == AF_INET)
//			{
//				while (remoteHost->h_addr_list[i] != 0) {
//					addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
//					printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
//				}
//			}
//			else if (remoteHost->h_addrtype == AF_NETBIOS)
//			{
//				printf("NETBIOS address was returned\n");
//			}
//		}
//	}
//
//	// Initiate connection on correct port
//
//	// Send request conforming to correct protocol
//
//	// Receive response
//
//	// Close socket
//	closesocket(sock);
//
//	return 1;
//}