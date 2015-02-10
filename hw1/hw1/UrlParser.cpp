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
	//printf("\nParsing URL: %s\n", url);
	Parameters *p = ((Parameters*)pParam);

	const char* hostname;
	const char* subrequest;
	int port, numLinks;

	bool extractedURL, robotSuccess, didCrawlUrl;

	if ((hostname = getHostname(url)) == NULL) {
		//printf("failed to parse host\n");
		return;
	}
	extractedURL = true;

	subrequest = getSubrequest(url, hostname);
	port = getPort(url);

	// Create WebSocket
	WebSocket webSocket = WebSocket();
	webSocket.Setup((char*)hostname, port, pParam);

	// Check robots.txt to see if crawling is allowed
	if (webSocket.checkRobots(hostname)) {
		robotSuccess = true;

		FILE* file = webSocket.downloadPage(hostname, subrequest);
		if (file != NULL) {
			didCrawlUrl = true;
			HtmlParser parser = HtmlParser();
			numLinks = parser.parse(file, (char*)url);
		}
	}

	WaitForSingleObject(p->mutex, INFINITE);		// lock mutex
	if (extractedURL)
		(p->numExtractedURLs)++;
	if (robotSuccess)
		(p->numURLsPassedRobotCheck)++;
	if (didCrawlUrl)
		(p->numCrawledURLs)++;
	if (numLinks > 0)
		(p->numLinks) += numLinks;
	ReleaseMutex(p->mutex);							// unlock mutex

}

const char* URLParser::getHostname(const char* url)
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

const char* URLParser::getSubrequest(const char* url, const char* hostname)
{
	// TODO: safely parse the subrequest

	char* scheme = strtok((char*)url, "://");

	char* hostBegin;
	char* subrequest = new char[strlen(url)];

	if ((hostBegin = strstr((char*)url, hostname)) != NULL)
	{
		char* path = strtok(hostBegin, "/");
		char* extra = strtok(path, "?#");
		
		path -= strlen(extra);
		return path;
	}

	return "/";
}

int URLParser::getPort(const char* url)
{
	const char* colon;

	char* portString;
	int port = 80;

	if ((colon = strrchr(url, ':')) != NULL)
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
