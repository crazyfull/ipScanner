#include <iostream>
#include "Client.h"
#include <Wrench/clsTCPServer.h>
#include <Wrench/log.h>
#include <Wrench/clsFileDirectory.h>

using namespace std;

#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()


int main(int argc, char *argv[])
{
    LOG("CloudFlare tunnel Scanner");
    LOG("help: [start ip range] [milisec delay]\n");

    clsTCPServer *pServer = new clsTCPServer(1024 * 16);
    pServer->Start();

    if (argc < 2) {
        LOG("you must enter a ip address");
        return 1;
    }

    CString enteredIP = argv[1];
    int delay = 500;

    if(argc > 2){
        delay = atol(argv[2]);
    }


    auto lngTargetIP = inet_addr(enteredIP.Data());
    if(lngTargetIP <= 0 || lngTargetIP == UINT32_MAX){
        LOG("invalid input ip: [%s]", enteredIP.Data());
        return 1;
    }

    LOG("Scanning started: [%s] delay [%d]", enteredIP.Data(), delay);
    sleep(2);

    auto range3 = (lngTargetIP / 256*256*256 ) / (256*256*256);
    auto range4 = (lngTargetIP / 256*256 ) / (256*256*256);

    for(int b = 0; b < 256-range3;b++){

        for(int i = 0; i < 256-range4;i++){

            //range to ip string
            char strIP[INET_ADDRSTRLEN] = {0};
            inet_ntop(AF_INET, &lngTargetIP, strIP, sizeof(strIP));

            //
            //LOG("Clientstr: [%s] %u", strIP, targetIP);
            Client *pClient = new Client(pServer);
            pClient->Connect(strIP, 443);

            //
            lngTargetIP += 256*256*256;
            usleep(delay * 1000);
        }
        range4 = 0;
        lngTargetIP += 256*256;

    }

    getchar();
    LOG("end...");
    return 0;
}
