#include "include/trackerget.h"
#include "include/trackersend.h"

void init() {
	bool exit = false;

	int sockfd, newsockfd, portno, n;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    sockfd =  socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = atoi(firstTrackerPort.c_str());
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = INADDR_ANY;  
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    listen(sockfd, 5);

    clilen = sizeof(cli_addr);

	while(!exit) {

	    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	    if (newsockfd < 0) 
	        error("ERROR on accept");

	    bzero(buffer, 256);

	    n = read(newsockfd, buffer, 255);

	    if (n < 0) 
	        error("ERROR reading from socket");

	    printf("Signal: %s\n",buffer);

	    if(strcmp(buffer, "receive") == 0) {
	    	pid_t pid;

	    	if((pid = fork()) == 0) {
	    		close(sockfd);
	    		receiveMetaData(newsockfd);
	    		exit = true;
	    		continue;
	    	}

	    }

	    if(strcmp(buffer, "send") == 0) {
	    	pid_t pid;

	    	if((pid = fork()) == 0) {
	    		close(sockfd);
	    		sendSeederList(newsockfd);
	    		exit = true;
	    		continue;
	    	}
	    }

	    if(strcmp(buffer, "remove") == 0) {
	    	pid_t pid;

	    	if((pid = fork()) == 0) {
	    		close(sockfd);
	    		removeSeeder(newsockfd, seederFile);
	    		exit = true;
	    		continue;
	    	}
	    }
	}
	close(sockfd);
}

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
	firstTrackerIP = data;
	getline(str1, data, ':');
	firstTrackerPort = data;

	stringstream str2(argv[2]);
	getline(str2, data, ':');
	secondTrackerIP = data;
	getline(str2, data, ':');
	secondTrackerPort = data;

	seederFile = argv[3];

	logFile = argv[4];

    init();

    return 0;
}