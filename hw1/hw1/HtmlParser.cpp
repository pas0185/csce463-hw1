/*
* Patrick Sheehan
* CSCE463 HW1
* 26 January 2015
*
* References: CPSC 463 homework handout
*/

#include "HtmlParser.h"

// Return number of links found
int HtmlParser::parse(FILE* file, char* baseUrl, LPVOID pParam)
{
	// create new parser object
	HTMLParserBase *parser = new HTMLParserBase;

	Parameters *p = ((Parameters*)pParam);

	int nLinks;
	char *linkBuffer = parser->Parse(file->_base, (int)strlen(file->_base), baseUrl, (int)strlen(baseUrl), &nLinks);

	// check for errors indicated by negative values
	if (nLinks < 0)
		nLinks = 0;

	WaitForSingleObject(p->mutex, INFINITE);
	(p->numLinks) += nLinks;
	ReleaseMutex(p->mutex);

	//printf("done in %d ms with %d links\n", (1000 * total / CLOCKS_PER_SEC), nLinks);

	// print each URL; these are NULL-separated C strings
	for (int i = 0; i < nLinks; i++) {
		//printf("%s\n", linkBuffer);
		linkBuffer += strlen(linkBuffer) + 1;
	}

	delete parser;		// this internally deletes linkBuffer

	return nLinks;
}
