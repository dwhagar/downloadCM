#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>

#include "downloadfile.h"

using namespace std;

#ifdef WIN32
int downloadFile(string filename)
{
	// Code to download in a Win32 environment.
	return 0;
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