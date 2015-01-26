/*
 * Patrick Sheehan
 * CSCE463 HW1
 * 26 January 2015
 *
 */

#include <string.h>
#include <stdio.h>

void parseURL(char* url)
{
	/* URL format: scheme://host[:port][/path][?query][#fragment] */
	printf("-----------------------\n");
	printf("Parsing URL = %s\n", url);


	char* scheme = strstr(url, "://");
	if (scheme == NULL)
	{
		printf("ERROR: Invalid URL. Expected a scheme ending in '://'");
	}
	else
	{
		char* host = scheme + 3;
		printf("host = %s\n", host);

		char* hashtag = strstr(url, "#");
		char* slash = strstr(url, "/");
		char* colon = strstr(url, ":");
		char* question = strstr(url, "?");

		printf("scheme = %s\n", scheme);
		printf("hashtag = %s\n", hashtag);
		printf("slash = %s\n", slash);
		printf("colon = %s\n", colon);
		printf("question = %s\n", question);
	}

	printf("-----------------------\n\n\n");


}