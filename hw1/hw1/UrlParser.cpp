/*
 * Patrick Sheehan
 * CSCE463 HW1
 * 26 January 2015
 *
 */

#include "URLParser.h"

using namespace std;

void URLParser::parse(const char* url, LPVOID pParam)
{
	Parameters *p = ((Parameters*)pParam);

	const char* hostname;
	const char* subrequest;
	int port = 80;

	bool robotSuccess = false;
	bool didCrawlUrl = false;

	if ((hostname = parseHostFromURL(url)) != NULL) {
		// successfully parsed host
		subrequest = getSubrequest(url);
		port = getPort(url);
	}

	// Create WebSocket
	WebSocket webSocket = WebSocket();
	webSocket.Setup((char*)hostname, port, pParam);

	// Check robots.txt to see if crawling is allowed
	if (webSocket.checkRobots(hostname, pParam)) {

		// download and parse the requested page
		webSocket.downloadPageAndCountLinks(hostname, subrequest, url, pParam);
	}


	// **MOVE****
	WaitForSingleObject(p->mutex, INFINITE);		// lock mutex
	if (didCrawlUrl) {
		//printf("crawled url");
		(p->numCrawledURLs) += 1;
	}
	ReleaseMutex(p->mutex);							// unlock mutex
}

const char* URLParser::parseHostFromURL(const char* url)
{
	const char* delim;
	char* hostname = new char[strlen(url)];

	if ((delim = strstr(url, "://")) != NULL)
	{
		// Stripping the scheme
		strcpy(hostname, delim + 3);

		const char* delimiters = ":/?#";
		size_t hostLength = strcspn(hostname, delimiters);

		hostname[hostLength] = '\0';

		//printf("host: %s, ", hostname);

		return hostname;
	}

	return NULL;
}

const char* URLParser::getSubrequest(const char* url)//, const char* hostname)
{
	const char* delim;
	char* request;

	if ((delim = strstr(url, "://")) != NULL)
	{
		// Stripping the scheme
		delim += 3;
		if ((delim = strstr(delim, "/")) != NULL) {

			size_t requestLength = strcspn(delim, "?#");
			request = new char[requestLength + 1];

			strncpy(request, delim, requestLength);
			request[requestLength] = '\0';
			return request;
		}
	}

	return "/";
}

int URLParser::getPort(const char* url)
{
	const char* colon;
	const char* delim;

	char* portString;
	int port = 80;

	if ((delim = strstr(url, "://")) != NULL)
	{
		// Stripping the scheme
		delim += 3;

		if ((colon = strstr(delim, ":")) != NULL)
		{
			for (int len = 0; len < strlen(colon); len++)
			{
				if (!isdigit(colon[len]))
				{
					portString = new char[len];
					strncpy(portString, colon + 1, len);
					istringstream in(portString);
					if (in >> port && in.eof())
					{
						break;
					}
				}
			}
		}
	}

	//printf("port: %d", port);
	return port;
}

char* URLParser::buildGETRequest(char* host, char* port, char* request)
{
	// Can't do anything without the host name
	if (host == NULL)
	{
		//printf("Failed to create a GET request. Expected char* for hostname, received NULL");
		return NULL;
	}

	// Assign default values if not provided
	if (request == NULL || request == " ")
		request = "/";

	if (port == NULL || port == " ")
		port = "80";

	// Build formatted request string
	int size = strlen(host) + strlen(port) + strlen(request) + strlen(useragent) + 50;
	char* GETReq = new char[size];
	sprintf(GETReq, "GET %s HTTP/1.0\r\nUser-agent: %s\r\nHost: %s\r\nConnection: close\r\n\r\n\0", request, useragent, host);

	return GETReq;
}

char* URLParser::parseURLString(char* url)
{

	//printf("        Parsing URL...");
	if (url == NULL)
	{
		//printf("url == NULL, aborting.\n");
		return NULL;
	}

	int newLen;
	char* delim, *host, *port, *request;
	char* tempUrl = new char[strlen(url)];

	if ((delim = strstr(url, "://")) != NULL)
	{
		// Stripping the scheme
		strcpy(tempUrl, delim + 3);

		// Stripping the fragment
		if ((delim = strstr(tempUrl, "#")) != NULL)
		{
			newLen = strlen(tempUrl) - strlen(delim);
			tempUrl[newLen] = '\0';
		}

		// Getting the request
		if ((delim = strstr(tempUrl, "/")) != NULL)
		{
			newLen = strlen(tempUrl) - strlen(delim);
			request = new char[strlen(delim)];
			strcpy(request, delim);
			tempUrl[newLen] = '\0';
		}
		else if ((delim = strstr(tempUrl, "?")) != NULL)
		{
			newLen = strlen(tempUrl) - strlen(delim);
			request = new char[strlen(delim) + 1];
			sprintf(request, "/%s", delim);
			//strcpy(request, delim);
			tempUrl[newLen] = '\0';
		}
		else
		{
			request = "/";
		}

		// Getting the port
		if ((delim = strstr(tempUrl, ":")) != NULL)
		{
			delim++;
			newLen = strlen(delim);
			port = new char[newLen];
			strcpy(port, delim);
			tempUrl[strlen(tempUrl) - newLen - 1] = '\0';
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
		}
		else
		{
			return NULL;
		}
	}

	//printf("host: %s, ", host);
	//printf("port: %s, ", port);
	//printf("request: %s\n", request);

	return buildGETRequest(host, port, request);
}
