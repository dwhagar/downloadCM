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
#include <ctime>

using namespace std;

// Code for compiling under anything other than Win32
#ifndef WIN32
#include <curl/curl.h>
const char pathEnd = '/';

// This function gives an unused function warning,
// but is actually used.
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded);
#endif

// Win32 code, sets proper path ending character constant
#ifdef WIN32
#pragma comment ( lib, "Wininet.lib" )

const char pathEnd = '\\';

void progressBar(double downloaded, double total);
#endif

// Both the Win32 and the Linux/Mac Code should fit into this prototype.
// which under MacOS/Linux will call write_data above.
int downloadFile(string url, string filename, int verbose);

#endif