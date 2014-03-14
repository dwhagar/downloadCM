#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>

#include "downloadfile.h"

using namespace std;

#ifdef WIN32
#include <Windows.h>
#include <WinInet.h>

int downloadFile(string url, string filename, int verbose)
{
	string clientID = "downloadCM";
	// This should set asside resources and let me open up a connection to
	// the Internet, using clientID as the HTTP client name (Mozilla, etc..)
	// Might need to 'borrow' a recognized client ID string from a normal browser.
	HINTERNET netCon = InternetOpen(
		clientID.c_str(),
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0);
	
	// Extract the server name from the URL
	// protocol is 7 characters, looks for the next / after the protocol text
	// one character behind that will be the last character of the URL
	// (pos - 8 + 1 to determine the length of the string to be pulle dout.
	size_t endOfServerName = url.find(8, url.find('/', 8));
	string serverName = url.substr(8, endOfServerName - 9);
	string resourceName = url.substr(endOfServerName + 1, url.length() - endOfServerName);
	
	// Couldn't use strings here, as it is an array of char arrays.  It is hard
	// coded, however, so not worried about memory issues.
	char *mimeTypes[2];
	mimeTypes[0] = "text/*";
	mimeTypes[1] = "application/*";

	HINTERNET httpCon = InternetConnect(
		netCon,
		serverName.c_str(),
		INTERNET_DEFAULT_HTTP_PORT,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,
		0,
		NULL);
	HINTERNET httpReq = HttpOpenRequest(
		httpCon,
		"GET",
		resourceName.c_str(),
		NULL,
		NULL,
		mimeTypes,
		0,
		NULL);
	bool sendReq = HttpSendRequest(
		httpReq,
		NULL,
		NULL,
		NULL,
		0);

	if (sendReq)
	{
		// Lets open that file now.
		ofstream outputFile;
		outputFile.open(filename.c_str(), ios::out);

		// Get ready to pull data from the Internet.
		bool readFile;
		byte dataIn[1024];
		LPDWORD readSize;

		// Here is our loop, should run until the server reports there is no more
		// file to read.
		do
		{
			readFile = InternetReadFile(
				httpReq,
				dataIn,
				1024,
				readSize);

			outputFile << dataIn;  // Should work like cout does, just dump data out
		} while ((readFile) && (readSize > 0));

		outputFile.close(); // Make sure we close the file.
	}

	// Handle clenaup
	InternetCloseHandle(httpReq);
	InternetCloseHandle(httpCon);
	InternetCloseHandle(netCon);
}
#endif

#ifndef WIN32
#include <curl/curl.h>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

int downloadFile(string url, string filename, int verbose)
{
	// Initialize all the memory we'll be needing for data.
	CURL *curl;
	FILE *bodyfile;

	// Initialize curl_Easy
	curl = curl_easy_init();

	// If curl initializes, lets try to get our update page.
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

		// Open the file we're going to put this stuff in.
		bodyfile = fopen(filename.c_str(), "wb");
		if (bodyfile == NULL) {
			curl_easy_cleanup(curl);
			return 1; // Failed to open the file.
		}
        
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, bodyfile);
		curl_easy_perform(curl);
		fclose(bodyfile);

		/* always cleanup */
		curl_easy_cleanup(curl);
		return 0; // Success
	}
    else
    {
        return 1; // Failed
    }
}
#endif