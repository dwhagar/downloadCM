//
//  argFlags.cpp
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

#include <string>

#include "argFlags.h"

using namespace std;

argFlags::argFlags()
{
	help = 0;
	isVerbose = 0;
	numDevices = 1; // Must be 1 to reflect size of the array in memory.
	string *deviceIDs = new string[1]; // Initialize array memory.
	deviceIDs[0] = " "; // Initialize array data.
}

// Copy constructor
argFlags::argFlags(const argFlags &data)
{
	help = data.help;
	isVerbose = data.isVerbose;
	numDevices = data.numDevices;
	delete[] deviceIDs;
	string *deviceIDs = new string[numDevices];
	for(int count = 0; count < numDevices; count++)
	{
		deviceIDs[count] = data.deviceIDs[count];
	}
}

argFlags::~argFlags()
{
	// Cleanup memory.
	delete[] deviceIDs;
}

void argFlags::setVerbose(int yesNo)
{
	isVerbose = yesNo;
}

void argFlags::setNumber(int number)
{
	if (number == numDevices)
	{
		// Data is unchanged, do nothing.
		return;
	}
	else
	{
		// Create a new string array and copy existing data to it.
		string *temp = new string[numDevices];
		for(int count = 0; count < numDevices; count++)
		{
			temp[count] = deviceIDs[count];
		}
		
		// Delete the old array data now that it is copied.
		delete [] deviceIDs;
		
		// Redefine array and copy in old data.
		string *deviceIDs = new string[number];
		for(int count = 0; count < numDevices; count++)
		{
			deviceIDs[count] = temp[count];
		}
		
		// Set the parameter now that the array is the correct size.
		numDevices = number;
		
		// Delete the temporary array.
		delete[] temp;
	}
}

void argFlags::setDevice(string id, int number)
{
	if ((number > numDevices) || (number < 0))
	{
		// Impossible, don't do anything.
		return;
	}
	else
	{
		deviceIDs[number] = id;
	}
	
}

void argFlags::setHelp(int number)
{
	help = number;
}

int argFlags::getVerbose()
{
	return isVerbose;
}

int argFlags::getNumber()
{
	return numDevices;
}

string argFlags::deviceID(int number)
{
	if (number > numDevices)
	{
		// Never try to access beyond the array.
		return deviceIDs[numDevices - 1];
	}
	else if (number < 0)
	{
		// Never try to access beyond the array.
		return deviceIDs[0];
	}
	else
	{
		return deviceIDs[number];
	}
}

int argFlags::getHelp()
{
	return help;
}