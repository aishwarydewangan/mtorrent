#include "include/clientshare.h"

void makeSHAVector(string fileName) {

    unsigned char digest[SHA_DIGEST_LENGTH]; 

    char buffer[BLOCK];

    FILE *fs = fopen(fileName.c_str(), "r");

    if(fs == NULL)
        error("Error: File not found");

    bzero(buffer,BLOCK);

    int fileBlock;

    SHA_CTX ctx;

    SHA1_Init(&ctx);

    while((fileBlock = fread(buffer, sizeof(char), BLOCK, fs)) > 0) {

        SHA1((unsigned char*)&buffer, fileBlock, (unsigned char*)&digest); 

        SHA1_Update(&ctx, buffer, fileBlock);

        char tempMDString[SHA_DIGEST_LENGTH*2+1];

        for(int i = 0; i < SHA_DIGEST_LENGTH; i++)
            sprintf(&tempMDString[i*2], "%02x", (unsigned int)digest[i]);

        string temp = tempMDString;

        shaVector.push_back(temp.substr(0, 20).c_str());

        bzero(buffer,BLOCK);
    }

    SHA1_Final(digest, &ctx);

    char mdString[SHA_DIGEST_LENGTH*2+1];

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    string str = mdString;
 
    shaVector.push_back(str.substr(0, 20).c_str());

    fclose(fs);
}

void createMetaData(string fileName) {
    if(!isFileExists(fileName.c_str()))
        error("Error");

    string fileSize;

    string mtdt = "temp.mtdt";

    struct stat st; 

    if (stat(fileName.c_str(), &st) == 0)
        fileSize = to_string(st.st_size);

    FILE *fp = fopen(mtdt.c_str(), "w");

    fprintf(fp, "%s\n", shaVector.back().c_str());

    fprintf(fp, "127.0.0.1\n");

    fprintf(fp, "12345\n");

    fprintf(fp, "%s\n", fileName.c_str());

    fprintf(fp, "%s", fileSize.c_str());

    fclose(fp);
}

void removeMetaData(string mtdt) {
    if(isFileExists(mtdt.c_str()))
        remove(mtdt.c_str());
}

void makeTorrent(string fileName, string mtorrent, string) {

    if(!isFileExists(fileName.c_str()))
        error("Error");

    string fileSize;

    if(isFileExists(mtorrent.c_str()))
        return;

    struct stat st; 

    if (stat(fileName.c_str(), &st) == 0)
        fileSize = to_string(st.st_size);

    FILE *fp = fopen(mtorrent.c_str(), "w");

    fprintf(fp, "127.0.0.1:12345\n");

    fprintf(fp, "127.0.0.1:12346\n");

    fprintf(fp, "%s\n", fileName.c_str());

    fprintf(fp, "%s\n", fileSize.c_str());

    for(int i = 0; i < shaVector.size(); i++) {
        fprintf(fp, "%s\n", shaVector[i].c_str());
    }

    fclose(fp);
}

void share(string fileName, string mtorrent, string trackerPort) {
    string signal = "receive";
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
    
    bzero(buffer,256);

    n = write(sockfd, signal.c_str(), signal.size());

    if (n < 0) 
         error("ERROR writing to socket");

    bzero(buffer, 256);

    n = read(sockfd, buffer, 255);

    if (n < 0) 
         error("ERROR reading from socket");

    if(strcmp(buffer, "rok") == 0) {

        makeSHAVector(fileName);

        makeTorrent(fileName, mtorrent);

        createMetaData(fileName);

        string mtdt = "temp.mtdt";

        FILE *fs = fopen(mtdt.c_str(), "r");

        if(fs == NULL)
            error("Error: File not found.");

        bzero(buffer, 256);

        int fileBlock;

        write(sockfd, mtdt.c_str(), mtdt.size());

        bzero(buffer, 256);

        n = read(sockfd, buffer, 255);

        if (n < 0) 
             error("ERROR reading from socket");

         if(strcmp(buffer, "fok") == 0) {
            while((fileBlock = fread(buffer, sizeof(char), 256, fs)) > 0) {
                n = write(sockfd, buffer, fileBlock);
                bzero(buffer, 256);
            }
         }
        fclose(fs);

        removeMetaData(mtdt);

    } else {
        printf("\nNegative ack");
    }

    close(sockfd);
}

void removeTorrent(string mtorrent, string trackerPort) {

    if(!isFileExists(mtorrent.c_str()))
        error("Error");

    string signal = "remove";
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
    
    bzero(buffer,256);

    n = write(sockfd, signal.c_str(), signal.size());

    if (n < 0) 
         error("ERROR writing to socket");

    bzero(buffer, 256);

    n = read(sockfd, buffer, 255);

    if (n < 0) 
         error("ERROR reading from socket");

    if(strcmp(buffer, "rok") == 0) {
        bzero(buffer, 256);

        int fileBlock;

        FILE *fs = fopen(mtorrent.c_str(), "r");

        while((fileBlock = fread(buffer, sizeof(char), 256, fs)) > 0) {
            n = write(sockfd, buffer, fileBlock);
            bzero(buffer, 256);
        }

        fclose(fs);

        remove(mtorrent.c_str());
    } else {
        printf("\nNegative ack");
    }

    close(sockfd);
}