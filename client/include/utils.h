#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <openssl/sha.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <dirent.h>
#include <sys/stat.h>
#include <arpa/inet.h> 

using namespace std;

#define BLOCK 524288

static string firstTrackerIP, firstTrackerPort;

static string secondTrackerIP, secondTrackerPort;

static string clientIP, uploadPort;

static string logFile;

inline void error(const char *msg)
{
    perror(msg);
    exit(0);
}

inline bool isFileExists(const char *name) {
    ifstream file(name);
    bool check = file.good();
    file.close();
    return check;
}

#endif