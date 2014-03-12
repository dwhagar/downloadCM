//
//  cmUpdate.cpp
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

#include <string>
#include <ctime>

#include "cmUpdate.h"

using namespace std;

const long MyPort = 80;

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
		return isUpdated;
	}
}

string cmUpdate::getURL()
{
	return updateURL;
}

string cmUpdate::getID()
{
	return deviceID;
}