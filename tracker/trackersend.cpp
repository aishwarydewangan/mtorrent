#include "include/trackersend.h"

void sendSeederList(int newsockfd) {
    int n;

    char buffer[256];

    send(newsockfd, "sok\n", 3, 0);

    bzero(buffer, 256);

    n = read(newsockfd, buffer, 255);

    if (n < 0) 
        error("ERROR reading from socket"); 

    string sha(buffer);

    ifstream in(seederFile.c_str());

    string str;

    vector<string> seederList;

    while(getline(in, str)) {
        if(sha.substr(0, 20).compare(str.substr(0, 20)) == 0) {
            stringstream ss(str);
            string token, temp;
            getline(ss, token, ' ');
            getline(ss, token, ' ');
            temp = token;
            getline(ss, token, ' ');
            temp = temp + ":" + token;
            seederList.push_back(temp);
        }
    }

    in.close();

    if(seederList.size()==0)
        send(newsockfd, "seedneg\n", 7, 0);
    else {
        send(newsockfd, "seedok\n", 6, 0);

        bzero(buffer, 256);

        read(newsockfd, buffer, 255);

        if(strcmp(buffer, "size") == 0) {
            send(newsockfd, to_string(seederList.size()).c_str(), to_string(seederList.size()).size(), 0);

            bzero(buffer, 256);

            read(newsockfd, buffer, 255);

            if(strcmp(buffer, "sizeok") == 0) {
                for(int i = 0; i < seederList.size(); i++) {
                    send(newsockfd, seederList[i].c_str(), seederList[i].size(), 0);

                    bzero(buffer, 256);

                    read(newsockfd, buffer, 255);

                    if(strcmp(buffer, "ok") == 0) {
                        continue;
                    } else
                        error("Error");
                }       
            }
        }
    }

    close(newsockfd);
}