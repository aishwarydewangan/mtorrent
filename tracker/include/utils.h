#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

#define BLOCK 524288

static string firstTrackerIP, firstTrackerPort;

static string secondTrackerIP, secondTrackerPort;

static string seederFile, logFile;

inline void error(const char *msg)
{
    perror(msg);
    exit(1);
}

inline bool isFileExists(const char *name) {
    ifstream file(name);
    bool check = file.good();
    file.close();
    return check;
}

#endif