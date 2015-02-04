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
#include "FileParser.h"
#include "UrlParser.h"

using namespace std;

FileParser::FileParser()
{
}


FileParser::~FileParser()
{
}

void FileParser::parse(std::string filename, int threadCount)
{
	ifstream infile(filename);
	string url;

	// TODO faster: read file into one buffer and separate on '\n'

	// Break down file into several URLs
	while (getline(infile, url))
	{
		// Parse each URL
		URLParser::parse(url.c_str());
	}

	return;
}