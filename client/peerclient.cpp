#include "include/peerclient.h"


void getFileNew(vector<string> seeders) {
	stringstream ss(seeders.front());
    string token;
    getline(ss, token, ':');

    string ip = token;

    getline(ss, token, ':');

    string port = token;

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = stoi(port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname("localhost");

    if (server == NULL) {
        fprintf(stderr,"Receive: ERROR, no such host\n");

        fprintf(stderr,"IP: %s\tPort: %s\n", ip.c_str(), port.c_str());

        fprintf(stderr,"IP Size: %lu\n", ip.size());

        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    //serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    string fileName = "linux.mp4";

    char buffer[256];

    FILE *fr = fopen(fileName.c_str(), "a");

    bzero(buffer,256);

    write(sockfd, fileName.c_str(), fileName.size());

    n = read(sockfd, buffer, 256);

    if (n < 0) 
        error("ERROR reading from socket");

    if(strcmp(buffer, "ok") == 0) {

        bzero(buffer,256);

        int fileBlock;

        while((fileBlock = read(sockfd, buffer, 256)) > 0) {
            if (fileBlock < 0) 
                error("ERROR reading from socket");

            int writeSize = fwrite(buffer, sizeof(char), fileBlock, fr);

            if(writeSize < fileBlock)
                error("Cannot write file. Something is wrong.");
        }
    }

    fclose(fr);
    
    close(sockfd);
}

void receiveFile(vector<string> seeders) {

    stringstream ss(seeders.front());
    string token;
    getline(ss, token, ':');

    string ip = token;

    getline(ss, token, ':');

    string port = token;

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = atoi(port.c_str());
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname("localhost");
    //server = gethostbyaddr(ip, sizeof(ip), AF_INET);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    string fileName = "linux.mp4";

    char buffer[256];

    FILE *fr = fopen(fileName.c_str(), "a");

    bzero(buffer,256);

    write(sockfd, fileName.c_str(), fileName.size());

    n = read(sockfd, buffer, 256);

    if (n < 0) 
        error("ERROR reading from socket");

    if(strcmp(buffer, "ok") == 0) {

        bzero(buffer,256);

        int fileBlock; 

        write(sockfd, "now\n", 3);

        bzero(buffer, 256);

        while((fileBlock = read(sockfd, buffer, 256)) > 0) {
            if (fileBlock < 0) 
                error("ERROR reading from socket");

            int writeSize = fwrite(buffer, sizeof(char), fileBlock, fr);

            if(writeSize < fileBlock)
                error("Cannot write file. Something is wrong.");
        }
    }

    fclose(fr);
    
    close(sockfd);
}