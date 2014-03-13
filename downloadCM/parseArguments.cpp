//
//  parseArguments.cpp
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

#include <string>
#include <iostream>
#include <vector>
#include "parseArguments.h"

using namespace std;

argFlags parseArguments(int number, char **arguments)
{
	// Create a string array to copy arguments into, since strings are easier
	// to work with.
	vector<string> cmdArgs(number);
	// Create the return variable.
	argFlags flags;
	// Keep track of number of devices.
	int devices = 0;
    string tempStr;
	
	// Walk through the arrays and copy each argument into the string array.
	for (int count = 0; count < number; count++)
	{
		cmdArgs.at(count) = arguments[count];
		// Now lets decide what they are and set flags.
		if (cmdArgs.at(count) == "-h")
		{
			flags.setHelp(1);
		}
		else if (cmdArgs.at(count) == "-v")
		{
			flags.setVerbose(1);
		}
		else if (cmdArgs.at(count) == "-d")
		{
            tempStr = getArgPair(count, number, cmdArgs);
            if (tempStr != " ")
            {
                devices++; // found a device, keep track of how many
				flags.setNumber(devices);
				flags.setDevice(tempStr, devices);
                count++; // bump the count, since we found the device
				         // flag and it's paired ID
            }
		}
        else if (cmdArgs.at(count) == "-p") // Set the location for download
        {
            tempStr = getArgPair(count, number, cmdArgs);
            if (tempStr != " ")
            {
				flags.setLocation(tempStr);
                count++; // bump count, since this is an argument pair
            }
        }
        else if (cmdArgs.at(count) == "-s")  // Use subfolders for downloads
        {
            flags.setSubFolder(1);
        }
	}
	
	// Can't do this without any devices specified.
	if ((devices == 0) && (flags.getHelp() == 0))
	{
		cout << "You did not specify any devices, please review the help." << endl << endl;
		flags.setHelp(1);
	}

	return flags;
}


// Checks to make sure they are enough arguments passed to fill in the other side of the
// argument pair, and returns the next item.
string getArgPair(int number, int max, vector<string> arguments)
{
    if (number + 1 < max)
    {
        return arguments.at(number + 1);
    }
    else
    {
        return " ";
    }
}