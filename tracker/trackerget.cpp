#include "include/trackerget.h"

void addSeeder(string fileName) {

    FILE *seederList = fopen("seeder.list", "a");

    ifstream mtdt(fileName.c_str());

    string data, temp;

    mtdt >> temp;

    data = temp;

    while(mtdt >> temp) {
        data = data + " " + temp;
    }

    data = data + "\n";

    fwrite(data.c_str(), sizeof(char), data.size(), seederList);

    mtdt.close();

    fclose(seederList);
}

void removeSeeder(int newsockfd, string sFile) {
    int n;

    char buffer[256];

    send(newsockfd, "rok\n", 3, 0);

    bzero(buffer, 256);

    string fileName = "temp.mtorrent";

    FILE *fr = fopen(fileName.c_str(), "w");

    int fileBlock;

    while((fileBlock = read(newsockfd, buffer, 256)) > 0) {
        if (fileBlock < 0) 
            error("ERROR reading from socket");

        int writeSize = fwrite(buffer, sizeof(char), fileBlock, fr);

        if(writeSize < fileBlock)
            error("Cannot write file. Something is wrong.");

        bzero(buffer, 256);
    }

    fclose(fr);

    ifstream in("temp.mtorrent");

    string fName;

    in >> fName;
    in >> fName;
    in >> fName;

    string sha;

    while(in >> sha);

    in.close();

    ifstream seederList(sFile.c_str());

    ofstream out("temp.list");

    string data;

    while(getline(seederList, data)) {

        if(!strstr(data.c_str(), fName.c_str()) && !strstr(data.c_str(), fName.c_str()))
            out << data << "\n";
            
    }

    seederList.close();

    out.close();

    remove("temp.mtorrent");

    remove(sFile.c_str());

    rename("temp.list", sFile.c_str());
}

void removeMetaData(string fileName) {
    if(isFileExists(fileName.c_str()))
        remove(fileName.c_str());
}

void receiveMetaData(int newsockfd) {
    int n;

    char buffer[256];

    send(newsockfd, "rok\n", 3, 0);

    bzero(buffer, 256);

    n = read(newsockfd, buffer, 255);

    if (n < 0) 
        error("ERROR reading from socket");

    int fileBlock = 0;

    string fileName = buffer;

    FILE *fr = fopen(fileName.c_str(), "w");

    bzero(buffer, 256);

    send(newsockfd, "fok\n", 3, 0);

    while((fileBlock = read(newsockfd, buffer, 256)) > 0) {
        if (fileBlock < 0) 
            error("ERROR reading from socket");

        int writeSize = fwrite(buffer, sizeof(char), fileBlock, fr);

        if(writeSize < fileBlock)
            error("Cannot write file. Something is wrong.");

        bzero(buffer, 256);
    }

    fclose(fr);

    addSeeder(fileName);

    removeMetaData(fileName);

    close(newsockfd);
}