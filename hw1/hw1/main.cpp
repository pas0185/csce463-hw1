/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*
* References:
*   http://www.cplusplus.com/reference/cstdio/fread/
*   http://stackoverflow.com/questions/16867372/splitting-strings-by-newline-character-in-c
*   http://stackoverflow.com/questions/7868936/read-file-line-by-line
*   http://stackoverflow.com/questions/2029103/correct-way-to-read-a-text-file-into-a-buffer-in-c
*   CSCE 463 Sample Code by Dimitri Loguinov
*/

#include "Headers.h"

#define FILE_READER_THREAD 0
#define STATS_THREAD 1
#define FIRST_CRAWLER_THREAD 2

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
	char* inputFile;
};

UINT fileThreadFunction(LPVOID pParam)
{	
	// Producer - adds URLs to a queue shared among threads
	
	Parameters *p = ((Parameters*)pParam);

	// safely get file name from shared parameters
	WaitForSingleObject(p->mutex, INFINITE);				// lock mutex
	printf("File thread started\n");						// critical section
	HANDLE hFile = CreateFile(p->inputFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	ReleaseMutex(p->mutex);									// unlock mutex
	
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed with %d\n", GetLastError());
		return 0;
	}

	// get file size
	LARGE_INTEGER li;
	BOOL bRet = GetFileSizeEx(hFile, &li);
	// process errors
	if (bRet == 0)
	{
		printf("GetFileSizeEx error %d\n", GetLastError());
		return 0;
	}

	// read file into a buffer
	int fileSize = (DWORD)li.QuadPart;			// assumes file size is below 2GB; otherwise, an __int64 is needed
	DWORD bytesRead;
	// allocate buffer
	char *fileBuf = new char[fileSize];
	// read into the buffer
	bRet = ReadFile(hFile, fileBuf, fileSize, &bytesRead, NULL);
	// process errors
	if (bRet == 0 || bytesRead != fileSize)
	{
		printf("ReadFile failed with %d\n", GetLastError());
		return 0;
	}

	// done with the file
	CloseHandle(hFile);

	// safely add files to shared queue
	WaitForSingleObject(p->mutex, INFINITE);			// lock
	char *url = strtok(fileBuf, "\r\n");
	while (url) {
		p->urlQueue.push(url);							// push url into queue
		url = strtok(0, "\r\n");
	}
	ReleaseMutex(p->mutex);								// unlock
	

	// print we're about to exit
	WaitForSingleObject(p->mutex, INFINITE);
	printf("File Thread finishing execution\n");
	ReleaseMutex(p->mutex);

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
	if (argc != 3) {
		printf("Invalid number of command line args provided\n");
		printf("Usage:\n\t> hw1.exe <NUM-THREADS> <URL-INPUT.TXT>");
		return 0;
	}

	int numThreads;
	std::string fileName;

	if (sscanf(argv[1], "%d", &numThreads) != 1) {
		printf("Error parsing number of threads, assuming 1\n");
		printf("Usage:\n\t> hw1.exe <NUM-THREADS> <URL-INPUT.TXT>");
		numThreads = 1;
	}

	fileName = argv[2];

	// initialize shared data structures & parameters sent to threads
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
	// assign input file that contains URLs
	p.inputFile = (char *)fileName.c_str();


	// start file-reader thread
	handles[FILE_READER_THREAD] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fileThreadFunction, &p, 0, NULL);

	// start stats thread
	handles[STATS_THREAD] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)statThreadFunction, &p, 0, NULL);
	
	// start N crawling threads

	//URLParser::parse(url.c_str());


	// wait for file-reader thread to quit
	WaitForSingleObject(handles[FILE_READER_THREAD], INFINITE);

	// wait for N crawling threads to finish

	// signal stats thread to quit; wait for it to terminate
	SetEvent(p.eventQuit);
	WaitForSingleObject(handles[STATS_THREAD], INFINITE);

	// cleanup

	system("pause");
	return 0;
}
