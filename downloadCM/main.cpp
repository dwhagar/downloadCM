//
//  main.cpp
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#ifndef WIN32
#include <curl/curl.h>
#endif

#include "main.h"
#include "argFlags.h"
#include "cmUpdate.h"
#include "parseArguments.h"
#include "downloadFile.h"

using namespace std;

// This application is designed to download updates for CM devices.
// Device downloads are looked up based on the code for the device
// such as n5110 is the code for the Galaxy Note 8.0.

int main(int argc, char **argv)
{
	argFlags flags; // Initialize where we're going to store flags.
#ifndef WIN32
	curl_global_init(CURL_GLOBAL_ALL); // Initialize cURL library.
#endif

	if (argc <= 1) // Requires at least 1 argument to run properly.
	{
		cout << "Please specify what you'd like me to do, use the -h option for help." << endl;
		return 1;
	}
	else if (argc > 1)
	{
		flags = parseArguments(argc, argv);
	}

	if (flags.getHelp() == 1)
	{
		// User wahts to know how to use the program, call help.
		displayHelp();
		return 1;
	}
	
	// Copy device ID's into array of cmUpdate objects, then check for updates.
	int number = flags.getNumber();
	int result = 2;
	vector<cmUpdate> devices;
	devices.resize(number);
    string tempLocation;
	
	for(int count = 0; count < number; count++)
	{
		// Rather than walk through the array twice, do this all at once.
		devices.at(count).setID(flags.deviceID(count));
		result = devices.at(count).checkUpdate(0);
	
		switch(result) 
		{
			case 0: // No update found.
				if (flags.getVerbose() == 1)
				{
					cout << "No update found for " << devices.at(count).getID() << "." << endl;
				}
				break;
			case 1:  // Update found.
				if (flags.getVerbose() == 1)
				{
					cout << "Update found for device " << devices.at(count).getID() << " at URL:" << endl;
					cout << devices.at(count).getURL() << endl;
                    cout << "Attempting to download update file " << devices.at(count).getFilename() << "." << endl;
                    cout << "Using download path below with subfolder download set to " << flags.getSubFolder() << "." << endl;
                    cout << flags.getLocation() << endl;
				}
                if (flags.getSubFolder() == 1)
                {
                    tempLocation = flags.getLocation() + devices.at(count).getID() + pathEnd;
                }
                else
                {
                    tempLocation = flags.getLocation();
                }
                devices.at(count).downloadUpdate(tempLocation, flags.getVerbose());
            break;
			case 2: // Update not checked (this should not happen unless there is an error).
				if (flags.getVerbose() == 1)
				{
					cout << "Did not check for update for device " << devices.at(count).getID() << "." << endl;
				}
            break;
			default: // Got something back that we didn't know how to handle.
				if (flags.getVerbose() == 1)
				{
					cout << "Something went wrong checking for device " << devices.at(count).getID() << "." << endl;
				}				
		}
	}
	
	// Will need some kind of code here to save the updated state, prevent
	// the computer from hitting the server when we've already gotten the
	// update.  Add to cmUpdate class?
	
	return 0;
}

void displayHelp()
{
	cout << "This program attempts to download updates for supported CyanogenMod devices." << endl;
	cout << "It was designed to be run via cron and in its default state, only displays" << endl;
	cout << "errors and not successes." << endl << endl;
	cout << "Usage:  downloadCM [-v] [-p path] [-s] -d id [-d id2] [-d id3]" << endl;
	cout << " -v     Verbose Mode, Log activity to the console." << endl;
	cout << " -d id  Specify the CyanogenMod device ID to check on." << endl;
    cout << " -p     Sets the path for the download." << endl;
    cout << " -s     Specifies the program should download updates to sub-folders by the" << endl;
    cout << "        device ID specified above." << endl;
	cout << " -h     Display this help message." << endl << endl;
	cout << "You must specify at least one device ID with the -d option, otherwise this" << endl;
	cout << "message displays.  Unless you specify the -v option, this program is silent." << endl << endl;
}