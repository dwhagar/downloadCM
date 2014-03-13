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

using namespace std;

argFlags::argFlags()
{
	help = 0;
	isVerbose = 0;
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
	if (number == numDevices)
	{
		// Data is unchanged, do nothing.
		return;
	}
	else
	{
		// Resize the vector and set the new size.
		deviceIDs.resize(number);
		numDevices = number;
	}
}

void argFlags::setDevice(string id, int number)
{
	if ((number > numDevices - 1) || (number < 0))
	{
		// Impossible, don't do anything.
		return;
	}
	else
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
		// Never try to access beyond the array.
		return deviceIDs.at(numDevices - 1);
	}
	else if (number < 0)
	{
		// Never try to access beyond the array.
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