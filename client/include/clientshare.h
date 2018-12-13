#ifndef CLIENTSHARE_H
#define CLIENTSHARE_H

#include "utils.h"

static vector<string> shaVector;

void makeSHAVector(string);

void createMetaData(string);

void removeMetaData(string);

void makeTorrent(string, string);

void share(string, string, string);

void removeTorrent(string, string);

#endif