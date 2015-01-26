// hw1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void parseURLsFromFile(char* fileName);

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc > 1) {
		parseURLsFromFile(argv[1]);
	}
	
	printf("Press enter key to continue");
	scanf("\n");
	return 0;
}

