#include "Client.h"
#include <Wrench/log.h>
#include <Wrench/clsCString.h>
#include <Wrench/clsFileDirectory.h>


Client::Client(clsTCPServer *pServer): clsTCPSocket(pServer) {

}

void Client::OnConnectFailed(const char *msg, int errCode)
{
    //LOG("OnConnectFailed()");
}

void Client::OnConnecting()
{
    LOG("Connecting(%s)", m_targetHost.Data());
}

void Client::OnConnected()
{
    //LOG("OnConnected()");
    clsTCPSocket::Send("HEAD / HTTP/1.0\r\n\r\n");
}

void Client::OnReceiveData(const char *Buffer, int Length)
{
    CString Data(Buffer, Length);

    /*
HTTP/1.1 400 Bad Request
Server: cloudflare
Date: Sat, 17 Aug 2024 14:31:33 GMT
Content-Type: text/html
Content-Length: 253
Connection: close
CF-RAY: -
*/

    if(Data.isExist("Server: ") == true){
        CString svName = CString::SplitString(Data, "Server: ","\r");

        if(svName.isCompare("cloudflare")){
            isSuccess = true;
            LOG_COLOR(Green , "OK(%s)\n", m_targetHost.Data());

            SaveIPToFile();
        }
    }

    clsTCPSocket::Close(true);
}

void Client::OnClosed()
{
    if(!isSuccess)
        LOG("OnClosed()");

    delete this;
}

void Client::SaveIPToFile()
{
    CString fData = FileDirectory::ReadFile("ips.txt");

    fData.append(m_targetHost);
    fData.append("\r\n");

    FileDirectory::SaveFile(fData.Data(),"ips.txt");
}

void Client::Connect(const char *HostAddress, uint16_t Port)
{
    m_targetHost = HostAddress;
    clsTCPSocket::ConnectToHost(HostAddress, Port, TIMEOUT_7Sec);
}

