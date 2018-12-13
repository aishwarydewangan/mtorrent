#include "include/clientget.h"
#include "include/clientshare.h"
#include "include/peerserver.h"

int main(int argc, char *argv[]) {

	if(argc < 5) {
		error("Insufficient Arguments");
	}

	if(argc > 5) {
		error("Extra Arguments");
	}

	string data;

	stringstream str1(argv[1]);
	getline(str1, data, ':');
	clientIP = data;
	getline(str1, data, ':');
	uploadPort = data;

	stringstream str2(argv[2]);
	getline(str2, data, ':');
	firstTrackerIP = data;
	getline(str2, data, ':');
	firstTrackerPort = data;

	stringstream str3(argv[3]);
	getline(str3, data, ':');
	secondTrackerIP = data;
	getline(str3, data, ':');
	secondTrackerPort = data;

	logFile = argv[4];

	pid_t pid;

	if((pid = fork()) == 0) {
		init();
		exit(0);
	}

    bool exit = false;

    while(!exit) {
    	string ip, cmd;

    	cout << "\n$$ ";
    	getline(cin, ip);

    	stringstream ss(ip);
	    getline(ss, cmd, ' ');

	    if(cmd == "exit") {
	    	exit = true;
	    	continue;
	    }

	    if(cmd == "share") {

	    	getline(ss, cmd, ' ');
    
    		string fileName = cmd;

    		getline(ss, cmd, ' ');

    		string mtorrent = cmd;

	    	pid_t pid;

	    	if((pid = fork()) == 0) {
	    		share(fileName, mtorrent, firstTrackerPort);
	    		exit = true;
	    		continue;
	    	}
	    }

	    if(cmd == "get") {

	    	getline(ss, cmd, ' ');
    
    		string mtorrent = cmd;

    		getline(ss, cmd, ' ');

    		string dest = cmd;

	    	pid_t pid;

	    	if((pid = fork()) == 0) {
	    		receiveSeederList(mtorrent, firstTrackerPort);
	    		exit = true;
	    		continue;
	    	}
	    }

	    if(cmd == "remove") {

	    	getline(ss, cmd, ' ');
    
    		string mtorrent = cmd;

	    	pid_t pid;

	    	if((pid = fork()) == 0) {
	    		removeTorrent(mtorrent, firstTrackerPort);
	    		exit = true;
	    		continue;
	    	}
	    }
    }

    return 0;
}