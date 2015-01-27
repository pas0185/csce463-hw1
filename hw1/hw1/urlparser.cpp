/*
 * Patrick Sheehan
 * CSCE463 HW1
 * 26 January 2015
 *
 */

#include "UrlParser.h"

// Defined in requestmaker.cpp
char* buildGETRequest(char* host, char* port, char* request);

char* UrlParser::parseURLString(char* url)
{
	printf("\tParsing URL...");
	if (url == NULL)
	{
		printf("url == NULL, aborting.\n");
		return NULL;
	}

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
			tempUrl[newLen] = '\0';
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
			newLen = strlen(tempUrl);
			host = new char[newLen];
			strcpy(host, tempUrl);
			strcpy(hostname, tempUrl);
		}
		else
		{
			return NULL;
		}
	}

	printf("host: %s, ", host);
	printf("port: %s, ", port);
	printf("request: %s\n", request);

	return buildGETRequest(host, port, request);
}

char* UrlParser::getLastHostName()
{
	if (hostname == NULL)
		return NULL;

	return hostname;
}