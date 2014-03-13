//
//  argFlags.cpp
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

#include <string>
#include <vector>

#include "argFlags.h"
#include "downloadFile.h"

using namespace std;

argFlags::argFlags()
{
	help = 0;
	isVerbose = 0;
    downloadLocation = "./"; // Use the current working directory.
    subFolder = 1; // Default is to use subfolders.
	numDevices = 1; // Must be 1 to reflect size of the array in memory.
	deviceIDs.resize(1);
	deviceIDs.at(0) = " ";
}

// Copy constructor
argFlags::argFlags(const argFlags &data)
{
	help = data.help;
	isVerbose = data.isVerbose;
	numDevices = data.numDevices;
	deviceIDs.resize(deviceIDs.size());
    downloadLocation = data.downloadLocation;
    subFolder = data.subFolder;
	for(int count = 0; count < numDevices; count++)
	{
		deviceIDs.at(count) = data.deviceIDs.at(count);
	}
}

argFlags::~argFlags()
{
	// Nothing to see here.
}

void argFlags::setVerbose(int yesNo)
{
	isVerbose = yesNo;
}

void argFlags::setNumber(int number)
{
	if (number != numDevices)
	{
		deviceIDs.resize(number);
		numDevices = number;
	}
}

// Makes sure that element number for the device is within
// the proper range.
void argFlags::setDevice(string id, int number)
{
	if ((number < numDevices) && (number >= 0))
	{
		deviceIDs.at(number) = id;
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
		// Never try to access beyond the vector size.
		return deviceIDs.at(numDevices - 1);
	}
	else if (number < 0)
	{
		// Never try to access beyond the vector size.
		return deviceIDs.at(0);
	}
	else
	{
		return deviceIDs.at(number);
	}
}

int argFlags::getHelp()
{
	return help;
}

string argFlags::getLocation()
{
    return downloadLocation;
}

int argFlags::getSubFolder()
{
    return subFolder;
}

void argFlags::setLocation(string location)
{
    if (location.back() == pathEnd) // Good the user was smart
    {
        downloadLocation = location;
    }
    else // If no trailing slash or backslash is appent one
    {
        downloadLocation = location + pathEnd;
    }
}

void argFlags::setSubFolder(int useSubFolder)
{
    if (useSubFolder < 0)
    {
        subFolder = 0;
    }
    else
    {
        subFolder = 1;
    }
}

