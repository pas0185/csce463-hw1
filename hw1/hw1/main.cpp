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
	char *url = strtok(fileBuf, "\r\n");
	while (url) {
		WaitForSingleObject(p->mutex, INFINITE);		// lock
		//printf("<-Producing %s\n", url);

		p->urlQueue.push(url);							// push url into queue
		ReleaseMutex(p->mutex);							// unlock
		ReleaseSemaphore(p->semaphoreCrawlers, 1, NULL);	// increment semaphore by 1

		url = strtok(0, "\r\n");
	}

	SetEvent(p->eventFileReadFinished);

	// print we're about to exit
	WaitForSingleObject(p->mutex, INFINITE);
	printf("File Thread finishing execution\n");
	ReleaseMutex(p->mutex);

	return 0;
}
UINT statThreadFunction(LPVOID pParam)
{
	// Output stats every 2 seconds. 

	Parameters *p = ((Parameters*)pParam);

	clock_t currClock, lastClock = clock();

	while (WaitForSingleObject(p->eventQuit, 500) == WAIT_TIMEOUT)
	{
		WaitForSingleObject(p->mutex, INFINITE);
		currClock = clock();
		double secondsSinceReport = ((double)(lastClock - currClock)) / CLOCKS_PER_SEC;
		int totalSeconds = ((double)(lastClock - p->clock)) / CLOCKS_PER_SEC;

		double pps = p->numCrawledURLs / secondsSinceReport;
		double mbps = (p->numBytesDownloaded / 1000.0) / secondsSinceReport;
		lastClock = currClock;

		printf(
			"[%3d] %6d Q %7d E %6d H %6d D %5d I %5d R %5d C %4d L\n",
			totalSeconds,
			p->urlQueue.size(),
			p->numExtractedURLs,
			p->numURLsWithUniqueHost,
			p->numSuccessfulDNSLookups,
			p->numURLsWithUniqueIP,
			p->numURLsPassedRobotCheck,
			p->numCrawledURLs,
			p->numLinks
			);

		printf("   *** crawling %.1f pps & %.1f Mbps\n", pps, mbps);

		ReleaseMutex(p->mutex);
	}

	WaitForSingleObject(p->mutex, INFINITE);
	// print final results
	double totalSeconds = ((double)(clock() - p->clock)) / CLOCKS_PER_SEC;

	printf("\nRESULTS\n");
	printf("Extracted %d URLs @ %d/s\n", p->numExtractedURLs, p->numBytesDownloaded / totalSeconds);
	printf("Looked up %d DNS names @ %d/s\n", p->numSuccessfulDNSLookups, (int)(p->numSuccessfulDNSLookups / totalSeconds));
	printf("Downloaded %d robots @ %d/s\n", p->numURLsPassedRobotCheck, (int)(p->numURLsPassedRobotCheck / totalSeconds));
	printf("Crawled %d pages @ %d/s (%.2f GB)\n", p->numCrawledURLs, p->numCrawledURLs / totalSeconds, p->numBytesDownloaded / 1000000.0);
	printf("Parsed %d links @ %d/s\n", p->numLinks, p->numLinks / totalSeconds);
	printf("HTTP codes: 2xx = %d, 3xx = %d, 4xx = %d, 5xx = %d, other = %d\n", 
		p->code2xxCount, p->code3xxCount, p->code4xxCount, p->code5xxCount, p->codeOtherCount);

	ReleaseMutex(p->mutex);
	return 0;
}
UINT crawlerThreadFunction(LPVOID pParam)
{
	// Consumer - removes URLs from shared queue for processing
	Parameters *p = ((Parameters*)pParam);
	char* url;

	HANDLE arr[] = { p->semaphoreCrawlers, p->eventFileReadFinished };
	int size = sizeof(arr) / sizeof(HANDLE);

	while (WaitForMultipleObjects(size, arr, false, INFINITE) == WAIT_OBJECT_0)
	{
		WaitForSingleObject(p->mutex, INFINITE);		// lock mutex

		if (p->urlQueue.empty())
			break;

		const char* tempURL = p->urlQueue.front().c_str();

		int length = strlen(tempURL) + 1;
		url = new char[length];
		strncpy(url, tempURL, length);
		url[length - 1] = '\0';

		p->urlQueue.pop();

		ReleaseMutex(p->mutex);							// unlock mutex

		URLParser parser = URLParser();
		parser.parse((const char*)url, pParam);

		ReleaseSemaphore(p->semaphoreCrawlers, 1, NULL);

	}

	WaitForSingleObject(p->mutex, INFINITE);		// lock mutex
	SetEvent(p->eventQuit);
	ReleaseMutex(p->mutex);							// unlock mutex

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
	HANDLE fileThread, statThread;
	HANDLE *crawlerThreads = new HANDLE[numThreads];
	Parameters p;

	// assign the time this thread is starting
	p.clock = clock();

	// create a mutex for accessing critical sections (including printf); initial state = not locked
	p.mutex = CreateMutex(NULL, 0, NULL);

	// create a semaphore that counts the number of active threads; initial value = 0, max = numThreads
	p.semaphoreCrawlers = CreateSemaphore(NULL, 0, numThreads, NULL);

	// create a quit event; manual reset, initial state = not signaled
	p.eventQuit = CreateEvent(NULL, true, false, NULL);

	// set to true once file is finished being read
	p.eventFileReadFinished = CreateEvent(NULL, true, false, NULL);

	// create a shared queue of URLs to be parsed
	p.urlQueue = std::queue<std::string>();

	// assign input file that contains URLs
	p.inputFile = (char *)fileName.c_str();

	// start file-reader thread
	fileThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fileThreadFunction, &p, 0, NULL);
	
	// start stats thread
	statThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)statThreadFunction, &p, 0, NULL);
	
	// start N crawling threads
	for (int i = 0; i < numThreads; i++) {
		crawlerThreads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)crawlerThreadFunction, &p, 0, NULL);
	}

	// wait for file-reader thread to quit
	WaitForSingleObject(p.eventFileReadFinished, INFINITE);

	// wait for N crawling threads to finish
	WaitForMultipleObjects(numThreads, crawlerThreads, TRUE, INFINITE);
	// TODO: maybe need to wait and CloseHandle one by one


	// signal stats thread to quit; wait for it to terminate
	SetEvent(p.eventQuit);

	WaitForSingleObject(statThread, INFINITE);

	// cleanup

	system("pause");
	return 0;
}
