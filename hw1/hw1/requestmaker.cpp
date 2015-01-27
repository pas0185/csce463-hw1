/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*
*/

#include "stdafx.h"

char* GETRequest(char* scheme, char* host, char* port, char* path, char* query, char* fragment)
{
	if (host == NULL)
		return NULL;

	int requestSize = (path != NULL) ? strlen(path) : 0
					+ (query != NULL) ? strlen(query) : 0;
	char* request = (char *)malloc(requestSize * sizeof(char));
	
	int size = (host != NULL) ? strlen(host) : 0
		+ requestSize
		+ 50;

	char* fullRequest = (char *)malloc(size * sizeof(char));
	sprintf(fullRequest, 
		"GET %s HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n", 
		request, host);
						
	return fullRequest;
}