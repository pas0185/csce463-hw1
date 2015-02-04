/*
 * Patrick Sheehan
 * CSCE463 HW1
 * 26 January 2015
 *
 */

#include "URLParser.h"
#include "WebSocket.h"
#include "HtmlParser.h"

using namespace std;


const char* getRequest(const char* type, const char* host, int port, const char* request)
{
	//char* GETRequest[1024];

	// Can't do anything without the host name
	if (host == NULL) {
		printf("Failed to create a GET request. Expected char* for hostname, received NULL");
		return NULL;
	}

	// Assign default value if not provided
	if (request == NULL || request == " ")
		request = "/";

	// Build formatted request string
	int size = strlen(host) + strlen(request) + strlen(useragent) + 50;
	char* GETRequest = new char[size];
	sprintf(GETRequest, "%s %s HTTP/1.0\r\nUser-agent: %s\r\nHost: %s\r\nConnection: close\r\n\r\n\0", type, request, useragent, host);

	return GETRequest;
}

const char* getHostname(const char* url)
{
	const char* delim;
	char* hostname = new char[strlen(url)];

	if ((delim = strstr(url, "://")) != NULL)
	{
		// Stripping the scheme
		strcpy(hostname, delim + 3);

		const char* delimiters = ":/?#";
		size_t hostLength = strcspn(hostname, delimiters);

		hostname[hostLength /* + 1 */] = '\0';

		printf("host: %s, ", hostname);

		return hostname;
	}

	return NULL;
}
const char*
int DNSLookup(const char* hostname)
{
	// Check if we've already done this look up
	// TODO: gethostbyname() here
	return 1234;
}

void URLParser::parse(const char* url)
{
	printf("\nURL: %s\n\tParsing URL...", url);

	// extract host name from URL
	const char* hostname;
	const char* subrequest;
	int port;

	if ((hostname = getHostname(url)) == NULL) {
		printf("failed to retrieve host, exiting now\n\n");
		return;
	}

	subrequest = getSubrequest(url);
	port = getPort(url);

	// Create WebSocket
	WebSocket webSocket = WebSocket(hostname);

				// TODO: move to WebSocket
				//Perform DNS Lookup to get IP address
				int IPAddress = DNSLookup(hostname);
				// ****


	// Use a HEAD request to get page statistics and check robots.txt
	const char* HEADRequest = getRequest("HEAD", hostname, port, subrequest);
	webSocket.Send(HEADRequest);
	webSocket.ReadHEADResponse();

				// Check HTTP code of HEAD response
				bool hasValidHeader = true;
				if (hasValidHeader) {
					// read robots.txt

					// read page statistics
				}
				else {
					// TODO: print full message and stop reading this URL
					cout << "Invalid header" << endl;
				}



	// Build GET request
	const char* GETRequest = getRequest("GET", hostname, port, subrequest);
	webSocket.Send(GETRequest);
	webSocket.ReadGETResponse();

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

	printf("\n");
}




char* URLParser::buildGETRequest(char* host, char* port, char* request)
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
	int size = strlen(host) + strlen(port) + strlen(request) + strlen(useragent) + 50;
	char* GETReq = new char[size];
	sprintf(GETReq, "GET %s HTTP/1.0\r\nUser-agent: %s\r\nHost: %s\r\nConnection: close\r\n\r\n\0", request, useragent, host);

	return GETReq;
}

char* URLParser::parseURLString(char* url)
{
	printf("\t  Parsing URL...");
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

char* URLParser::getLastHostName()
{
	if (hostname == NULL)
		return NULL;

	return hostname;
}

