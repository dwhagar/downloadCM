//
//  cmUpdate.h
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

// Facilitates obtaining an update from CyanogenMod's server for a single
// supported device.

// TODO:
//  Code update function.

#include <string>

using namespace std;

#ifndef CMUPDATE_H
#define CMUPFATE_H

class cmUpdate
{
	public:
		cmUpdate();
		~cmUpdate();
		void setID(string id);
		int checkUpdate(int force);
		string getURL();
		string getID();
        int downloadUpdate(int verbose);
		
	protected:
		string deviceID;
		string updateURL;
		string currentDate;
		string prefixURL;
		string checkURL;
        string updateFilename;
		int isUpdated;
        string parseHTML();
        string parseURL();
};

#endif