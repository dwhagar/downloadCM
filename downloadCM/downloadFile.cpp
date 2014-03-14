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
	string mimeType = "application/octet-stream";

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
		mimeType.c_str(),
		0,
		NULL);

	// Handle clenaup

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