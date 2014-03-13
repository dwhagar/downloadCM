//
//  downloadFile.h
//  downloadCM
//
//  Created by David Wade Hagar on 3/12/14.
//  Copyright (c) 2014 David Wade Hagar. All rights reserved.
//

#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <string>

using namespace std;

// Code for compiling under anything other than Win32
#ifndef WIN32
#include <curl/curl.h>

// This function gives an unused function warning,
// but is actually used.
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
#endif

int downloadFile(string url, string filename, int verbose);

#endif