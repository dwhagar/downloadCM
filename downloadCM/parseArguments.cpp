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
			// The -d option must have the device ID as the next argument.
			if (count + 1 < number)
			{
                cmdArgs.at(count + 1) = arguments[count + 1];
				devices++;
                flags.setNumber(devices);
				flags.setDevice(cmdArgs.at(count + 1), devices - 1);
				// Since we've just set the device ID, move skip that.
				count++;
			}
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