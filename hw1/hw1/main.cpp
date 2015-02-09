// hw1.cpp : Defines the entry point for the console application.
//

#include "Headers.h"

int HTMLParserTest();
void parseURLsFromFile(char* fileName);
void winsock_test(char* requestBuf);
void htmlParserTest();

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc == 3)
	{
		// > hw1.exe <NUM-THREADS> <URL-INPUT.TXT>

		// First param - thread count
		istringstream in(argv[1]);
		int numThreads;
		if (in >> numThreads && in.eof())
		{
			cout << "Number of threads to spawn: " << numThreads << endl;
		}
		else
		{
			numThreads = 0;
			cout << "Error parsing number of threads" << endl;
		}

		// Second param - file name
		string inputFile = argv[2];
		cout << "Provided input file: " << inputFile << endl;

		FileParser fileParser = FileParser();
		fileParser.parse(inputFile, numThreads);
	}
	else
	{
		cout << "Invalid number of arguments provided" << endl;
		cout << "Usage:\n\t> hw1.exe <NUM-THREADS> <URL-INPUT.TXT>" << endl;
	}

	cin.get();
	
	return 0;
}
