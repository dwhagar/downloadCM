//
//  cmUpdate.cpp
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

#include <curl/curl.h>

#include "cmUpdate.h"

using namespace std;

cmUpdate::cmUpdate()
{
	// Initialize blank or known values.
	deviceID = " ";
	updateURL = " ";
	prefixURL = "http://get.cm";
	checkURL = "http://download.cyanogenmod.com/?device=";
	isUpdated = 2; // a value of 2 denotes that nothing has been cheched yet.
				   // while 1 means update found and 0 means no update.
	
	// Parse the date into something we can use.
	time_t now = time(0);
	tm *ltm = localtime(&now);
	int curYear = ltm->tm_year + 1900;
	int curMonth = ltm->tm_mon + 1;
	currentDate = curYear + curMonth + ltm->tm_mday;
	delete ltm;
}

cmUpdate::~cmUpdate()
{
	// Nothing to see here.
}

void cmUpdate::setID(string id)
{
	deviceID = id;
}

size_t cmUpdate::write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

// Check for an update, parameter of "forced" allows an update check to be
// forced even if an update check has already been done.
int cmUpdate::checkUpdate(int force)
{
	if ((isUpdated != 2) && (force == 0))
	{
		// Check has already been done, return isUpdated value unless forced.
		return isUpdated;
	}
	else
	{
        // Initialize all the memory we'll be needing for data.
        CURL *curl;
        static const char *bodyfilename = "html.out";
        FILE *bodyfile;
        
        // Initialize curl_Easy
        curl = curl_easy_init();
        
        // If curl initializes, lets try to get our update page.
        if(curl)
        {
            // Set the options we'll need to be using, including our URL.
            // This will be a mix of prefixURL and the deviceID
            string checkURL = prefixURL + deviceID;
            curl_easy_setopt(curl, CURLOPT_URL, checkURL.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

            // Open the file we're going to put this stuff in.
            bodyfile = fopen(bodyfilename,"wb");
            if (bodyfile == NULL) {
                curl_easy_cleanup(curl);
                return 3; // Error opening the file will return an unknown value.
            }
            
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, bodyfile);
            curl_easy_perform(curl);
            fclose(bodyfile);
            
            /* always cleanup */
            curl_easy_cleanup(curl);
            
            string testURL;
            // Parse HTML, return URL if one is found.
            testURL = parseHTML();
            if (testURL == " ")
            {
                isUpdated = 0;
            }
            else
            {
                isUpdated = 1;
                updateURL = testURL;
            }
        }
        else
        {
            // Something odd happened, lets get out of here.
            curl_easy_cleanup(curl);
            return 3; // Error initializeing.
        }
        
		return isUpdated;
	}
}

// Now lets parse the output file to find if an update has been release.
string cmUpdate::parseHTML()
{
    string result = " ";
    string temp = " ";

    ifstream html("html.out");
    if (html.is_open())
    {
        do
        {
            // Get a single bit of information from the file
            getline(html, temp);
            
            // Try to find what we're looking for
            // An http:// link with todays date in it
            size_t protoPOS = temp.find("http://");
            size_t datePOS = temp.find(currentDate);
            
            if ((protoPOS != -1) && (datePOS != -1))
            {
                // If we find the link extract it
                size_t urlEnd = temp.find(".zip", protoPOS) + 4;
                if ((urlEnd > protoPOS) && (urlEnd > datePOS))
                {
                    result = temp.substr(protoPOS, urlEnd - protoPOS);
                    return result;
                }
            }
        } while (!html.eof());
    }
    return result;
}

string cmUpdate::getURL()
{
	return updateURL;
}

string cmUpdate::getID()
{
	return deviceID;
}