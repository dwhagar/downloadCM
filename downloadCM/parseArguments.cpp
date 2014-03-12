//
//  parseArguments.cpp
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

#include <string>
#include <iostream>
#include "parseArguments.h"

using namespace std;

argFlags parseArguments(int number, char **arguments)
{
	// Create a string array to copy arguments into, since strings are easier
	// to work with.
	string *cmdArgs = new string[number];
	// Create the return variable.
	argFlags flags;
	// Keep track of number of devices.
	int devices = 0;
	
	// Walk through the arrays and copy each argument into the string array.
	for (int count = 1; count < number; count++)
	{
		cmdArgs[count] = arguments[count];
		// Now lets decide what they are and set flags.
		if (cmdArgs[count] == "-h")
		{
			flags.setHelp(1);
		}
		else if (cmdArgs[count] == "-v")
		{
			flags.setVerbose(1);
		}
		else if (cmdArgs[count] == "-d")
		{
			// The -d option must have the device ID as the next argument.
			if (count < number - 1)
			{
				devices++;
				if (flags.getNumber() < devices)
				{
					flags.setNumber(devices);
				}
				flags.setDevice(cmdArgs[count + 1], devices);
				// Since we've just set the device ID, move skip that.
				count++;
			}
			else
			{
				// There weren't enough arguments left to determine the
				// device being asked for, fail and display help.
				flags.setHelp(1);
			}
		}
	}
	
	// Can't do this without any devices specified.
	if (devices == 0)
	{
		cout << "You did not specify any devices, please review the help." << endl << endl;
		flags.setHelp(1);
	}
	
	delete[] cmdArgs;
	return flags;
}