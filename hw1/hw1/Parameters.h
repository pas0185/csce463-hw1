/*
* Patrick Sheehan
* CSCE463 HW1
* 27 January 2015
*
*/

class Parameters {
	// taken from 463-sample , main.cpp
	// this dedicated class is passed to all threads, acts as shared memory
public:
	HANDLE mutex;
	HANDLE semaphoreCrawlers;
	HANDLE eventQuit;
	HANDLE eventFileReadFinished;

	std::queue<std::string> urlQueue;

	std::set<std::string> visitedHostSet;
	std::set<in_addr> visitedIPSet;

	char* inputFile;
	clock_t clock;

	int numExtractedURLs = 0;
	int numURLsWithUniqueHost = 0;		// TODO
	int numSuccessfulDNSLookups = 0;	// TODO
	int numURLsWithUniqueIP = 0;		// TODO
	int numURLsPassedRobotCheck = 0;
	int numCrawledURLs = 0;
	int numLinks = 0;
};