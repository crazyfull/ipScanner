#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
#include <Wrench/clsTCPSocket.h>

#define IP_LIST_FILE_NAME ("iplist.txt")

class Client: private clsTCPSocket
{
    CString m_targetHost;
    int m_port;
    bool isSuccess = false;

    //overides
    void OnConnectFailed(const char* msg, int errCode);
    void OnConnecting();
    void OnConnected();
    void OnReceiveData(const char* Buffer, int Length);
    void OnClosed();

    void SaveIPToFile();
public:
    Client(clsTCPServer *pServer);
    void Connect(const char* HostAddress, uint16_t Port);
};

#endif // HTTPCLIENT_H
