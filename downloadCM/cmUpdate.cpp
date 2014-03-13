//
//  cmUpdate.cpp
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include "cmUpdate.h"
#include "downloadFile.h"

using namespace std;

cmUpdate::cmUpdate()
{
	// Initialize blank or known values.
	deviceID = " ";
	updateURL = " ";
    updateFilename = " ";
	prefixURL = "http://get.cm";
	checkURL = "http://download.cyanogenmod.com/?device=";
	isUpdated = 2; // a value of 2 denotes that nothing has been cheched yet.
				   // while 1 means update found and 0 means no update.
	
	// Parse the date into something we can use.
	time_t now = time(0);
	tm *ltm = localtime(&now);
	int curYear = ltm->tm_year + 1900;
	int curMonth = ltm->tm_mon + 1;
    
    string curMonthStr;
    string curDayStr;
    
    if (curMonth < 10)
    {
        curMonthStr = "0" + to_string(curMonth);
    }
    else
    {
        curMonthStr = to_string(curMonth);
    }
    if (ltm->tm_mday < 10)
    {
        curDayStr = "0" + to_string(ltm->tm_mday);
    }
    else
    {
        curDayStr = to_string(ltm->tm_mday);
    }
    currentDate = to_string(curYear) + curMonthStr + curDayStr;
}

cmUpdate::~cmUpdate()
{
	// Nothing to see here.
}

void cmUpdate::setID(string id)
{
	deviceID = id;
    checkURL= checkURL + deviceID;
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
		int result = downloadFile(checkURL, "html.out", 0);
        if (result != 0)
        {
            return 3;
        }
        else
        {
            updateURL = parseHTML();
            updateFilename = parseURL();
        }
        return isUpdated;
	}
}

int cmUpdate::downloadUpdate(int verbose)
{
    int result = downloadFile(updateURL, updateFilename, verbose);
    return result;
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
                    isUpdated = 1;
                    return result;
                }
            }
        } while (!html.eof());
    }
    isUpdated = 0;
    return result;
}

// Extracts the filename from the URL
string cmUpdate::parseURL()
{
    size_t slashPOS = updateURL.find("/cm");
    size_t zipPOS = updateURL.find(".zip");
    string result = updateURL.substr(slashPOS + 1, zipPOS + 4 - slashPOS + 1);
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