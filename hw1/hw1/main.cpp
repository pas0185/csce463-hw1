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

// taken from 463-sample , main.cpp
// this dedicated class is passed to all threads, acts as shared memory
class Parameters {
public:
	HANDLE mutex;
	HANDLE finished;
	HANDLE eventQuit;
	std::queue<std::string> urlQueue;
	std::string inputFile;
};

UINT fileThreadFunction(LPVOID pParam)
{		
	Parameters *p = ((Parameters*)pParam);

	WaitForSingleObject(p->mutex, INFINITE);
	printf("File thread %d started\n", GetCurrentThread());
	std::string fileName = std::string(p->inputFile);
	ReleaseMutex(p->mutex);

	std::ifstream infile(fileName, std::ifstream::ate | std::ifstream::binary);

	int filesize = infile.tellg();
	printf("File thread: read file with size %d\n", filesize);

	infile = std::ifstream(fileName);
	std::string url;

	while (getline(infile, url)) {
		WaitForSingleObject(p->mutex, INFINITE);
		p->urlQueue.push(url);
		ReleaseMutex(p->mutex);

	}
	ReleaseMutex(p->mutex);									// release critical section


	return 0;
}
UINT statThreadFunction(LPVOID pParam)
{
	return 0;

}
UINT crawlerThreadFunction(LPVOID pParam)
{

	return 0;
}
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
	
	// Referencing 463 Sample code
	HANDLE *handles = new HANDLE[numThreads + 2];

	Parameters p;

	// create a mutex for accessing critical sections (including printf); initial state = not locked
	p.mutex = CreateMutex(NULL, 0, NULL);
	// create a semaphore that counts the number of active threads; initial value = 0, max = 2
	p.finished = CreateSemaphore(NULL, 0, 2, NULL);
	// create a quit event; manual reset, initial state = not signaled
	p.eventQuit = CreateEvent(NULL, true, false, NULL);
	// create a shared queue of URLs to be parsed
	p.urlQueue = std::queue<std::string>();
	
	p.inputFile = inputFile;


	// start file-reader thread
	handles[FILE_READER_THREAD] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fileThreadFunction, &p, 0, NULL);

	// start stats thread

	// start N crawling threads

	URLParser::parse(url.c_str());


	// wait for file-reader thread to quit

	// wait for N crawling threads to finish

	// signal stats thread to quit; wait for it to terminate

	// cleanup




	system("pause");
	return 0;
}
