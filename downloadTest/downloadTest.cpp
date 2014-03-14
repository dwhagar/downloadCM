// downloadTest.cpp
// This is just to test the downloadFile functions.

#include <iostream>
#include <string>

#include "../downloadCM/downloadFile.h"

int main(int argc, char **argv)
{
	string webURL = "http://home.blazingumbra.com/index.shtml";
	string output = "index.shtml";
	cout << "Attempting to download " << webURL << endl;
	int result = downloadFile(webURL, output, 0);
	cout << "Result was " << result << endl;

	webURL = "http://home.blazingumbra.com/eclipse.png";
	output = "eclipse.png";
	cout << "Attempting to download " << webURL << endl;
	result = downloadFile(webURL, output, 0);
	cout << "Result was " << result << endl;

	return 0;
}

