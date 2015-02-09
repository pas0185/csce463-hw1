/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*
* References:
*   http://www.cplusplus.com/reference/cstdio/fread/
*   http://stackoverflow.com/questions/16867372/splitting-strings-by-newline-character-in-c
*   http://stackoverflow.com/questions/7868936/read-file-line-by-line
*/

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

	if (sscanf(argv[1], "%d", &numThreads) != 1) {
		printf("Error parsing number of threads, assuming 1\n");
		printf("Usage:\n\t> hw1.exe <NUM-THREADS> <URL-INPUT.TXT>");
		numThreads = 1;
	}

	if (sscanf(argv[2], "%s", &inputFile) != 1) {
		printf("Error parsing input file name\n");
		printf("Usage:\n\t> hw1.exe <NUM-THREADS> <URL-INPUT.TXT>");
		return 0;
	}

	// initialize shared data structures & parameters sent to threads
	ifstream infile(filename, std::ifstream::ate | std::ifstream::binary);
	string url;
	queue<string> urlQueue;

	int filesize = infile.tellg();

	printf("Main thread: read file with size %d\n", filesize);
	infile = ifstream(filename);

	// TODO faster: read file into one buffer and separate on '\n'

	// Break down file into several URLs
	while (getline(infile, url))
	{
		urlQueue
			// Parse each URL
			URLParser::parse(url.c_str());

		if (TESTING)
			return;
	}



	// start file-reader thread

	FileParser fileParser = FileParser();
	fileParser.parse(inputFile, numThreads);

	// start stats thread

	// start N crawling threads

	// wait for file-reader thread to quit

	// wait for N crawling threads to finish

	// signal stats thread to quit; wait for it to terminate

	// cleanup




	system("pause");
	return 0;
}
