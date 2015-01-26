/*
 * Patrick Sheehan
 * CSCE463 HW1
 * 26 January 2015
 *
 */

#include "stdafx.h"

#define MAX_URL_LENGTH 1000
// if no valid scheme found, return NULL
// otherwise, return the scheme and truncate from the pointer
char* extractScheme(char** url)
{
	char* symbol = strstr(*url, "://");
	
	if (symbol != NULL)
	{
		int len = strlen(*url) - strlen(symbol);
		char scheme[MAX_URL_LENGTH];
		memcpy(scheme, *url, len);
		scheme[len] = '\0';
		printf("scheme = %s\n", scheme);

		*url += len + 3;

		printf("During extraction: url = %s\n", *url);

		return scheme;
	}
	
	printf("ERROR: Expected a scheme identifier token ('://') in the URL but was not found\n");
	return NULL;
}

void parseURL(char* url)
{
	/* URL format: scheme://host[:port][/path][?query][#fragment] */
	printf("-----------------------\n");
	printf("Parsing URL = %s\n", url);

	char* scheme = extractScheme(&url);
	printf("After extraction: url = %s\n", url);
	//char* fragment = extractFragment(url);

	if (false)//(scheme)
	{
		// fragment
		char* hashtag = strrchr(url, '#');
		if (hashtag != NULL)
		{
			char* fragment = url;
			strncpy(fragment, hashtag + 1, strlen(hashtag) + 1);
			printf("fragment = %s", fragment);
			int fLen = strlen(fragment);
			url[strlen(url) - fLen] = '\0';
		}

		// query
		char* qmark = strrchr(url, '?');
		if (qmark != NULL)
		{
			char* query = url;
			strncpy(query, qmark + 1, strlen(qmark) + 1);
			printf("query = %s", query);
			int qLen = strlen(query);
			url[strlen(url) - qLen] = '\0';
		}

		// path
		char* slash = strstr(url, "/");
		if (slash != NULL)
		{
			char* path = url;
			strncpy(path, slash + 1, strlen(slash) + 1);
			printf("path = %s\n", path);
			int pLen = strlen(path);
			url[strlen(url) - pLen] = '\0';
		}

		// port
		char* colon = strstr(url, ":");
		if (colon != NULL)
		{
			char* port = url;
			strncpy(port, colon + 1, strlen(colon) + 1);
			printf("port = %s\n", port);
			int pLen = strlen(port);
			url[strlen(url) - pLen] = '\0';
		}

		// host
		char* host = url;
		strncpy(host, url, strlen(url));
		printf("host = %s\n", host);
	}


	printf("-----------------------\n\n\n");
}

void parseURLsFromFile(char* fileName)
{
	// Based on example from http://www.phanderson.com/files/file_read.html

	FILE* file = fopen(fileName, "r");
	if (file != NULL) {
		char line[MAX_URL_LENGTH];
		while (fgets(line, sizeof(line), file) != NULL) {
			parseURL(line);
		}
	}

	fclose(file);
}