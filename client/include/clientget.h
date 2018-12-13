#ifndef CLIENTGET_H
#define CLIENTGET_H

#include "utils.h"

static vector<string> seederList;

string getSHAFromTorrent(string);

void receiveSeederList(string, string);

#endif