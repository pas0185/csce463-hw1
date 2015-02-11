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
	std::set<std::string> visitedIPSet;

	std::string tamuIPString;
	std::string inputFile;
	clock_t clock;

	int numTAMUHostFound = 0;

	double numBytesDownloaded = 0;
	int code2xxCount = 0;
	int code3xxCount = 0;
	int code4xxCount = 0;
	int code5xxCount = 0;
	int codeOtherCount = 0;

	int numExtractedURLs = 0;
	int numURLsWithUniqueHost = 0;
	int numSuccessfulDNSLookups = 0;
	int numURLsWithUniqueIP = 0;
	int numURLsPassedRobotCheck = 0;
	int numCrawledURLs = 0;
	int numLinks = 0;
};