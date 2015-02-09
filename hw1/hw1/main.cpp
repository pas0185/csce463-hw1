// hw1.cpp : Defines the entry point for the console application.
//

#include "Headers.h"

int HTMLParserTest();
void parseURLsFromFile(char* fileName);
void winsock_test(char* requestBuf);
void htmlParserTest();

int _tmain(int argc, _TCHAR* argv[])
{
	// parse command line args
	int numThreads;
	std::string inputFile;

	if (sscanf(argv[1], "%d", &numThreads))
		printf("Number of threads to spawn: %d\n", numThreads);
	else {
		printf("Error parsing number of threads, assuming 1\n");
		printf("Usage:\n\t> hw1.exe <NUM-THREADS> <URL-INPUT.TXT>");
		numThreads = 1;
	}

	if (sscanf(argv[2], "%s", &inputFile)) {
		FileParser fileParser = FileParser();
		fileParser.parse(inputFile, numThreads);
	}
	else {
		printf("Error parsing input file name\n");
		printf("Usage:\n\t> hw1.exe <NUM-THREADS> <URL-INPUT.TXT>");
		return 0;
	}

	// initialize shared data structures & parameters sent to threads


	// start file-reader thread

	// start stats thread

	// start N crawling threads

	// wait for file-reader thread to quit

	// wait for N crawling threads to finish

	// signal stats thread to quit; wait for it to terminate

	// cleanup




	system("pause");
	return 0;
}
