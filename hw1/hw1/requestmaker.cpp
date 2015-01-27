/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*
*/

#include "Headers.h"

char* buildGETRequest(char* host, char* port, char* request)
{
	// Can't do anything without the host name
	if (host == NULL)
	{
		printf("Failed to create a GET request. Expected char* for hostname, received NULL");
		return NULL;
	}

	// Assign default values if not provided
	if (request == NULL || request == " ")
		request = "/";

	if (port == NULL || port == " ")
		port = "80";

	// Build formatted request string
	int size = strlen(host) + strlen(port) + strlen(request) + 50;
	char* GETReq = new char[size];
	sprintf(GETReq, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", request, host);
	//sprintf(GETReq, "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n\0", request, host);

	return GETReq;
}