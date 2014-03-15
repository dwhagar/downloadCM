#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include "downloadfile.h"

using namespace std;

#ifdef WIN32
#include <Windows.h>
#include <WinInet.h>

int downloadFile(string url, string filename, int verbose)
{
	int result = 0;
	string clientID = "downloadCM";
	// Extract the server name from the URL
	size_t endOfServerName = url.find('/', 8);
	string serverName = url.substr(7, endOfServerName - 7);
	string resourceName = url.substr(endOfServerName + 1);

	// Couldn't use strings here, as it is an array of char arrays.  It is hard
	// coded, however, so not worried about memory issues.
	LPCSTR mimeTypes[] = { "text/*", NULL };

	// This should set asside resources and let me open up a connection to
	// the Internet, using clientID as the HTTP client name (Mozilla, etc..)
	// Might need to 'borrow' a recognized client ID string from a normal browser.
	HINTERNET netCon = InternetOpen(
		clientID.c_str(),
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0);
	if (netCon != NULL)
	{
		HINTERNET httpCon = InternetConnect(
			netCon,
			serverName.c_str(),
			INTERNET_DEFAULT_HTTP_PORT,
			NULL,
			NULL,
			INTERNET_SERVICE_HTTP,
			0,
			NULL);
		if (httpCon != NULL)
		{
			HINTERNET httpReq = HttpOpenRequest(
				httpCon,
				"GET",
				resourceName.c_str(),
				NULL,
				NULL,
				mimeTypes,
				0,
				NULL);
			if (httpReq != NULL)
			{
				bool sendReq = HttpSendRequest(
					httpReq,
					NULL,
					NULL,
					NULL,
					0);

				if (sendReq == TRUE)
				{
					char queryBuffer[4096];
					DWORD querySize = 4069;
					LPDWORD querySizePtr = &querySize;
					bool queryInfo = HttpQueryInfo(
						httpReq,
						HTTP_QUERY_CONTENT_LENGTH,
						queryBuffer,
						querySizePtr,
						NULL);

					double fileSize = atof(queryBuffer);
					double fileSizeDownloaded = 0;

					// Lets open that file now.
					ofstream outputFile;
					outputFile.open(filename.c_str(), ios::binary | ios::trunc);

					// Get ready to pull data from the Internet.
					bool readFile;
					char dataIn[1024];
					DWORD dataRead;
					LPDWORD dataReadPtr = &dataRead;

					// We don't want to end up calling the progress bar too much so we
					// have to poll for the current time before the loop starts.
					time_t start = time(0);

					// Here is our loop, should run until the server reports there is no more
					// file to read.
					do
					{
						readFile = InternetReadFile(
							httpReq,
							dataIn,
							1024,
							dataReadPtr);

						fileSizeDownloaded += dataRead;

						// We don't want to keep calling the progress bar
						// exsessively so, we'll make sure we only do it once
						// a second.
						time_t timeNow = time(0);

						if ((verbose == 1) && (timeNow > start))
						{
							progressBar(fileSizeDownloaded, fileSize);
						}

						outputFile.write(dataIn, dataRead);

					} while ((readFile == TRUE) && (dataRead != 0));

					outputFile.close(); // Make sure we close the file.
					if (verbose == 1)
					{
						cout << endl;
					}
				}
				else
				{
					result = GetLastError();
				}
			}
			else
			{
				result = GetLastError();
			}
			InternetCloseHandle(httpReq);
		}
		else
		{
			result = GetLastError();
		}
		InternetCloseHandle(httpCon);
	}
	else
	{
		result = GetLastError();
	}
	InternetCloseHandle(netCon);

	return result;
}

void progressBar(double downloaded, double total)
{
	int barSize = 40;
	double downloadFraction = downloaded / total;
	int fracBarSize = round(downloadFraction * barSize);
	int fracEmptySize = barSize - fracBarSize;

	HANDLE curScreenOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	PCONSOLE_SCREEN_BUFFER_INFO screenInfo;
	bool conInfoResult = GetConsoleScreenBufferInfo(curScreenOutput, screenInfo);

	if (conInfoResult == true)
	{
		cout << conInfoResult << endl;
		for (int count = 0; count < fracBarSize; count++)
		{
			// Progress
		}
		for (int count = 0; count < fracEmptySize; count++)
		{
			// Whitespace
		}

		// Precentage done
	}
}

#endif

#ifndef WIN32
#include <curl/curl.h>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
{
    // how wide you want the progress meter to be
    int totaldotz=40;
    double fractiondownloaded = NowDownloaded / TotalToDownload;
    // part of the progressmeter that's already "full"
    int dotz = round(fractiondownloaded * totaldotz);
    
    // create the "meter"
    int ii=0;
    printf("%3.0f%% [",fractiondownloaded*100);
    // part  that's full already
    for ( ; ii < dotz;ii++) {
        printf("=");
    }
    // remaining part (spaces)
    for ( ; ii < totaldotz;ii++) {
        printf(" ");
    }
    // and back to line begin - do not forget the fflush to avoid output buffering problems!
    printf("]\r");
    fflush(stdout);
    return 0;
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
        
        // Verbose progress bar
        if (verbose == 0)
        {
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        }
        else
        {
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
        }
        
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
        
        // Handles the progress bar issue, though I'd like another solution.
        if (verbose ==1)
        {
            cout << endl;
        }
        
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