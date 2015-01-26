// hw1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void parseURL(char* url);

int _tmain(int argc, _TCHAR* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		parseURL(argv[i]);
		//printf("%s\n", argv[i]);
	}
	
	printf("Press enter key to continue");
	scanf("\n");
	return 0;
}

