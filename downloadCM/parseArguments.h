//
//  parseArguments.h
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

#include "argFLags.h"

// Parses command line arguments for downloadCM project and returns them
// as an argFlags object.

// TODO:
// * Add -o argument, file output destination

using namespace std;

#ifndef PARSEARGUMENTS_H
#define PARSEARGUMENTS_H

argFlags parseArguments(int number, char **arguments);

#endif