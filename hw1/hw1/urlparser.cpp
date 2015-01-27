/*
 * Patrick Sheehan
 * CSCE463 HW1
 * 26 January 2015
 *
 */

#include "stdafx.h"

//#define MAX_URL_LENGTH 1000

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

	return buildGETRequest(host, port, request);
}

// searches for the component at the beginning of the url
// if no valid component found, return NULL
// otherwise, return the component and truncate from the url
//char* extractFromFront(char** url, char* delimiter)
//{
//	char* symbol = strstr(*url, delimiter);
//
//	if (symbol != NULL)
//	{
//		int len = strlen(*url) - strlen(symbol);
//		char component[MAX_URL_LENGTH];
//		strcpy(component, *url);
//		component[len] = '\0';
//
//		*url += len + strlen(delimiter);
//
//		return component;
//	}
//
//	return NULL;
//}
//
//// searches for the component at the end of the url
//// if no valid component found, return NULL
//// otherwise, return the component and truncate from the url
//char* extractFromBack(char** url, char* delimiter)
//{
//	char* symbol = strstr(*url, delimiter);
//
//	if (symbol != NULL)
//	{
//		int len = strlen(symbol);
//		char* comp = strdup(symbol);
//		int urlLen = strlen(*url);
//
//		(*url)[urlLen - len] = '\0';
//
//		return comp;
//	}
//
//	return NULL;
//}
//void parseHost(char** url, char** host)
//{
//
//}

//char* parseURLString(char* url)
//{
//	printf("\tParsing URL...");
//	char* scheme = extractFromFront(&url, "://");
//
//	// TODO - Check for Http://
//
//	char* fragment = extractFromBack(&url, "#");
//	char* request = extractFromBack()
//	char* query = extractFromBack(&url, "?");
//	char* path = extractFromBack(&url, "/");
//	char* port = extractFromBack(&url, ":");
//	char* host = new char[strlen(url)];
//	strcpy(host, url);
//
//	printf("host %s, ", host);
//	printf("port %s, ", port);
//	printf("request %s", request);
//
//	return buildGetRequest(scheme, host, port, path, query, fragment);;
//}

void parseURLsFromFile(char* fileName)
{
	//// Based on example from http://www.phanderson.com/files/file_read.html
	//FILE* file = fopen(fileName, "r");
	//if (file != NULL) {
	//	char line[MAX_URL_LENGTH];
	//	while (fgets(line, sizeof(line), file) != NULL) {
	//		parseURLString(strtok(line, "\n"));
	//		//struct URL url =
	//		//char* req = buildGetRequest(url.host, url.path, url.query);

	//	}
	//}

	//fclose(file);
}