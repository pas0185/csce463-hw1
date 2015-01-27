/*
 * Patrick Sheehan
 * CSCE463 HW1
 * 26 January 2015
 *
 */

#include "stdafx.h"

#define MAX_URL_LENGTH 1000

char* GETRequest(char* scheme, char* host, char* port, char* path, char* query, char* fragment);

// searches for the component at the beginning of the url
// if no valid component found, return NULL
// otherwise, return the component and truncate from the url
char* extractFromFront(char** url, char* delimiter)
{
	char* symbol = strstr(*url, delimiter);

	if (symbol != NULL)
	{
		int len = strlen(*url) - strlen(symbol);
		char component[MAX_URL_LENGTH];
		strcpy(component, *url);
		component[len] = '\0';

		*url += len + strlen(delimiter);

		return component;
	}

	return NULL;
}

// searches for the component at the end of the url
// if no valid component found, return NULL
// otherwise, return the component and truncate from the url
char* extractFromBack(char** url, char* delimiter)
{
	char* symbol = strstr(*url, delimiter);

	if (symbol != NULL)
	{
		int len = strlen(symbol);
		char* comp = strdup(symbol);
		int urlLen = strlen(*url);

		(*url)[urlLen - len] = '\0';

		return comp;
	}

	return NULL;
}
//void parseHost(char** url, char** host)
//{
//
//}

char* parseURLString(char* url)
{
	/* URL format: scheme://host[:port][/path][?query][#fragment] */
	printf("-----------------------\n");
	printf("Parsing URL = %s\n\n", url);
	//char* host;
	char* scheme = extractFromFront(&url, "://");
	//parseHost(&url, &host);

	char* fragment = extractFromBack(&url, "#");
	char* query = extractFromBack(&url, "?");
	char* path = extractFromBack(&url, "/");
	char* port = extractFromBack(&url, ":");

	

	printf("scheme    = %s\n", scheme);
	printf("host	  = %s\n", url);
	printf("port	  = %s\n", port);
	printf("path	  = %s\n", path);
	printf("query	  = %s\n", query);
	printf("fragment  = %s\n", fragment);


	char* request = GETRequest(scheme, url, port, path, query, fragment);
	printf("%s", request);

	printf("-----------------------\n\n\n");
	
	return GETRequest(scheme, url, port, path, query, fragment);
}

void parseURLsFromFile(char* fileName)
{
	// Based on example from http://www.phanderson.com/files/file_read.html
	FILE* file = fopen(fileName, "r");
	if (file != NULL) {
		char line[MAX_URL_LENGTH];
		while (fgets(line, sizeof(line), file) != NULL) {
			parseURLString(strtok(line, "\n"));
			//struct URL url =
			//char* req = GETRequest(url.host, url.path, url.query);

		}
	}

	fclose(file);
}