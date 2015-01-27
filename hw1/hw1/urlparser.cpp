/*
 * Patrick Sheehan
 * CSCE463 HW1
 * 26 January 2015
 *
 */

#include "stdafx.h"

// Defined in requestmaker.cpp
char* buildGETRequest(char* host, char* port, char* request);

char* parseURLString(char* url)
{
	if (url == NULL)
		return NULL;

	int newLen;
	char* delim, *host, *port, *request;
	char* tempUrl = new char[strlen(url)];

	if ((delim = strstr(url, "://")) != NULL)
	{
		// Stripping the scheme
		strcpy(tempUrl, delim + 3);

		if ((delim = strstr(tempUrl, "#")) != NULL)
		{
			// Stripping the fragment
			newLen = strlen(tempUrl) - strlen(delim);
			tempUrl[newLen + 1] = '\0';
		}

		// Getting the request
		if ((delim = strstr(tempUrl, "/")) != NULL)
		{
			newLen = strlen(tempUrl) - strlen(delim);
			request = new char[strlen(delim)];
			strcpy(request, delim);
			tempUrl[newLen + 1] = '\0';
		}
		else
		{
			request = "/";
		}

		// Getting the port
		if ((delim = strstr(tempUrl, ":")) != NULL)
		{
			newLen = strlen(tempUrl) - strlen(delim);
			port = strtok(delim, "/?# ");
			tempUrl[newLen + 1] = '\0';
		}
		else
		{
			port = "80";
		}

		// Getting the host
		if (tempUrl != NULL)
		{
			host = new char[strlen(tempUrl)];
			strcpy(host, tempUrl);
		}
		else
		{
			return NULL;
		}
	}

	printf("host %s, ", host);
	printf("port %s, ", port);
	printf("request %s\n", request);

	return buildGETRequest(host, port, request);
}
