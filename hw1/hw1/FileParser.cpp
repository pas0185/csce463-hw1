#include "FileParser.h"
#include "UrlParser.h"

FileParser::FileParser()
{
}


FileParser::~FileParser()
{
}

void FileParser::parse(std::string inFile, int threadCount)
{

	// Break down file into several URLs
	// TODO: simple: fopen, fgets, fclose, ifstream, etc
	// TODO: faster: read file into one buffer and separate on '\n'
	// TODO: faster: fread, or ReadFile


	std::string testURL = "http://www.symantec.com/verisign/ssl-certificates";

	// Parse each URL
	// TODO: call static URLParser Method
	URLParser::parse(testURL);
}