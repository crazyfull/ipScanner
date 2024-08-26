#include "Client.h"
#include <iostream>
#include <Wrench/clsTCPServer.h>
#include <Wrench/log.h>
#include <Wrench/clsFileDirectory.h>


#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

using namespace std;

clsTCPServer *pServer = NULL;
int delay =  50;
int port = 443;

struct IP{
    CString str;
    in_addr_t addr;
};

IP GetIPAddress(const char*strIPaddress){
    IP ret;
    ret.addr = inet_addr(strIPaddress);
    if(ret.addr <= 0 || ret.addr == UINT32_MAX){
        ret.addr = 0;
        return ret;
    }
    ret.str = strIPaddress;

    return ret;
}

void CoonectToTarget(const char* IPAddress){
    LOG("Clientstr: [%s]", IPAddress);
    //Client *pClient = new Client(pServer);
    //pClient->Connect(IPAddress, port);
}

void Scann(const char *startIP, const char *endIP){

    IP fromIP = GetIPAddress(startIP);
    IP toIP = GetIPAddress(endIP);
    if(fromIP.addr == 0 || toIP.addr == 0){
        LOG("entred invalid address");
        return;
    }

    LOG("Start scanning from:[%s]    to:[%s]", startIP, endIP);

    auto range2 = (fromIP.addr / 256*256*256*256) / (256*256*256);
    auto range3 = (fromIP.addr / 256*256*256)     / (256*256*256);
    auto range4 = (fromIP.addr / 256*256)         / (256*256*256);

    uint32_t ScanIPNum = fromIP.addr;

    for(int b = 0; b < 256-range2;b++){
        for(int c = 0; c < 256-range3;c++){

            for(int d = 0; d < 256-range4;d++){

                //range to ip string
                char strIP[INET_ADDRSTRLEN] = {0};
                inet_ntop(AF_INET, &ScanIPNum, strIP, sizeof(strIP));

                //
                CoonectToTarget(strIP);

                if(ScanIPNum == toIP.addr){
                    LOG("scanning ended");
                    return;
                }

                //
                ScanIPNum += 256*256*256;
                usleep(delay * 1000);
            }

            range4 = 0;
            ScanIPNum += 256*256;
        }

        range3 = 0;
        ScanIPNum += 256;
    }


}

int main(int argc, char *argv[])
{
    LOG("CloudFlare tunnel Scanner");
    LOG("help:");
    LOG("scan with entered range: [Start IP range] [the end of the IP range]   [port]  [milisec delay]");
    LOG("scan with file [-f] [file path]\n");


/*
    argc = 5;
    argv[1] = "192.168.0.25";
    argv[2] = "192.165.18.15";
    argv[3] = "443";
    argv[4] = "100";
*/
    //create socket
    pServer = new clsTCPServer(1024 * 16);
    pServer->Start();

    LOG("argv2 [%s]", argv[1]);

    //load file
    if(CString::isCompare(argv[1], "-f") == true) {
        const char *filepath = argv[2];
        LOG("filepath [%s]", filepath);

        return 0;
    }

    if (argc < 3) {
        LOG("You must enter the start and end IP or load ip database file");
        return 1;
    }

    if(argc > 3){
        port = atol(argv[3]);
    }

    if(argc > 4){
        delay = atol(argv[4]);
    }


    //
    Scann(argv[1], argv[2]);


    //getchar();
    //LOG("end...");
    return 0;
}
