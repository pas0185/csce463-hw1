/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*
*/

#include "HtmlParser.h"

void HtmlParser::parse(char* filename, char* baseUrl)
{
	// Taken from CPSC 463 homework handout

	// open html file
	HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	// process errors
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//printf("CreateFile failed with %d\n", GetLastError());
		return;
	}

	// get file size
	LARGE_INTEGER li;
	BOOL bRet = GetFileSizeEx(hFile, &li);

	// process errors
	if (bRet == 0)
	{
		//printf("GetFileSizeEx error %d\n", GetLastError());
		return;
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
		//printf("ReadFile failed with %d\n", GetLastError());
		return;
	}

	// done with the file
	CloseHandle(hFile);

	clock_t start, end, total;
	//printf("      + Parsing page... ");
	start = clock();
	// create new parser object
	HTMLParserBase *parser = new HTMLParserBase;

	int nLinks;
	char *linkBuffer = parser->Parse(fileBuf, fileSize, baseUrl, (int)strlen(baseUrl), &nLinks);

	// check for errors indicated by negative values
	if (nLinks < 0)
		nLinks = 0;
	end = clock();
	total = (double)(end - start);

	//printf("done in %d ms with %d links\n", (1000 * total / CLOCKS_PER_SEC), nLinks);

	// print each URL; these are NULL-separated C strings
	for (int i = 0; i < nLinks; i++)
	{
		//printf("%s\n", linkBuffer);
		linkBuffer += strlen(linkBuffer) + 1;
	}

	delete parser;		// this internally deletes linkBuffer
	delete fileBuf;

}

// Return number of links found
int HtmlParser::parse(FILE* file, char* baseUrl)
{
	// Based off CPSC 463 homework handout

	clock_t start, end, total;
	//printf("      + Parsing page... ");
	start = clock();
	// create new parser object
	HTMLParserBase *parser = new HTMLParserBase;

	int nLinks;
	char *linkBuffer = parser->Parse(file->_base, (int)strlen(file->_base), baseUrl, (int)strlen(baseUrl), &nLinks);

	// check for errors indicated by negative values
	if (nLinks < 0)
		nLinks = 0;
	end = clock();
	total = (double)(end - start);

	//
	// *numLinks += nLinks
	//

	//printf("done in %d ms with %d links\n", (1000 * total / CLOCKS_PER_SEC), nLinks);

	// print each URL; these are NULL-separated C strings
	for (int i = 0; i < nLinks; i++)
	{
		//printf("%s\n", linkBuffer);
		linkBuffer += strlen(linkBuffer) + 1;
	}

	delete parser;		// this internally deletes linkBuffer

	return nLinks;
}