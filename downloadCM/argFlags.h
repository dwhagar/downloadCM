//
//  argFlags.h
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

// Loads command line arguments for downloadCM project into memory
// used by parseArguments function.

// TODO:
//  Restrict ability to set flags to unexpected values.
//  Add flag for file output destination.

#include <string>
#include <vector>

using namespace std;

#ifndef ARGFLAGS_H
#define ARGFLAGS_H

class argFlags
{
	public:
		argFlags();
		argFlags(const argFlags &data);
		~argFlags();
		void setVerbose(int yesNo);
		void setNumber(int number);
		void setDevice(string id, int number);
		void setHelp(int number);
		int getVerbose();
		int getNumber();
		string deviceID(int number);
		int getHelp();
		
	private:
		int help;
		int isVerbose;
		int numDevices;
		vector<string> deviceIDs;
};

#endif