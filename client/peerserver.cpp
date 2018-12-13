#include "include/peerserver.h"

void sendFile(int newsockfd) {
    int n;

    char buffer[256];

    bzero(buffer,256);

    n = read(newsockfd, buffer, 256);

    if (n < 0) 
        error("ERROR reading from socket");

    string fileName = buffer;

    FILE *fs = fopen(fileName.c_str(), "r");

    bzero(buffer,256);

    int fileBlock;

    write(newsockfd, "ok\n", 2);

    bzero(buffer,256);

    n = read(newsockfd, buffer, 256);

    if (n < 0) 
        error("ERROR reading from socket");

    if(strcmp(buffer, "now") == 0) {

        bzero(buffer,256);

        while((fileBlock = fread(buffer, sizeof(char), 256, fs)) > 0) {
            n = write(newsockfd, buffer, fileBlock);
            bzero(buffer,256);
        }
    }
    fclose(fs);

    close(newsockfd);

}

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
    portno = atoi(uploadPort.c_str());
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

        pid_t pid;

        if((pid = fork()) == 0) {
            close(sockfd);

            sendFile(newsockfd);

            exit = true;

            continue;

        }

    }
    close(sockfd);

}