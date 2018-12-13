#include "include/clientget.h"
#include "include/peerclient.h"

string getSHAFromTorrent(string fileName) {
    FILE *file = fopen(fileName.c_str(), "r");

    if(file == NULL)
        error("Error");

    char buffer[128];

    while(fgets(buffer, sizeof(buffer), file) != NULL);

    string fileSHA(buffer);

    fclose(file);

    return fileSHA;
}

void receiveSeederList(string fileName, string trackerPort) {

    string signal = "send";

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    portno = atoi(trackerPort.c_str());

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname("localhost");

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
    
    bzero(buffer, 256);

    n = write(sockfd, signal.c_str(), signal.size());

    if (n < 0) 
         error("ERROR writing to socket");

    bzero(buffer,256);

    n = read(sockfd, buffer, 255);

    if (n < 0) 
         error("ERROR reading from socket");

    if(strcmp(buffer, "sok") == 0) {

        bzero(buffer,256);

        string sha = getSHAFromTorrent(fileName);

        n = write(sockfd, sha.c_str(), sha.size());

        if (n < 0) 
            error("ERROR writing to socket");

        bzero(buffer,256);

        n = read(sockfd, buffer, 255);

        if (n < 0) 
            error("ERROR reading from socket");

        if(strcmp(buffer, "seedok") == 0) {

            write(sockfd, "size", 4);

            bzero(buffer, 256);

            n = read(sockfd, buffer, 255);

            if (n < 0) 
                error("ERROR reading from socket");

            string temp(buffer);

            int seederSize = stoi(temp);

            printf("\nThere are %d seeders", seederSize);

            write(sockfd, "sizeok", 6);

            for(int i = 0; i < seederSize; i++) {
                bzero(buffer, 256);

                n = read(sockfd, buffer, 255);

                if (n < 0) 
                    error("ERROR reading from socket");

                write(sockfd, "ok", 2);

                seederList.push_back(buffer);
            }

            receiveFile(seederList);
        }

        if(strcmp(buffer, "seedneg") == 0) {
            printf("\nThere are no seeders");
        }

    } else {
        printf("\nNegative ack");
    }

    close(sockfd);
        
}