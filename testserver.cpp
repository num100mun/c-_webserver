//
// Created by 18746 on 2023/11/28.
//
#include "include/CELLlog.h"
//#include "include/CELL.h"
#include "atomic"
#include "thread"
class server{
    SOCKET _sock = INVALID_SOCKET;
    atomic_int _dl{0};
    atomic_int _cc{0};
    char * _recvbuf = new char[100];
public:
    SOCKET InitSocket(){
        WORD ver = MAKEWORD(2, 2);
        WSADATA dat;
        WSAStartup(ver, &dat);
        _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        printf("create socket success\n");
    }
    void Bind(){
        sockaddr_in _sin = {};
        _sin.sin_family = AF_INET;
        _sin.sin_port = htons(4550);
        _sin.sin_addr.S_un.S_addr = INADDR_ANY;
        if(bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) != SOCKET_ERROR) printf("bind success\n");
    }
    void Listen(){
        if(listen(_sock, 5) != SOCKET_ERROR)printf("listen success\n");
    }
    void OnRun(){
        while(true){
            fd_set fdRead;
            FD_ZERO(&fdRead);
            FD_SET(_sock, &fdRead);
            timeval t = { 2,0};
            select(_sock + 1, &fdRead, 0, 0, &t);
            printf("server in onrun fd_count %d, recv data %d \n", _cc.load(), _dl.load());
            if (FD_ISSET(_sock, &fdRead)){
                FD_CLR(_sock, &fdRead);
                Accept();
            }

        }
    }
    void Accept(){
        sockaddr_in clientAddr = {};
        int nAddrLen = sizeof(sockaddr_in);
        SOCKET cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
        _cc+=1;
        thread t1(mem_fn(&server::Recv1), this ,cSock);
        t1.detach();

    }
    void Recv1(SOCKET sock){
        int nlen = (int) recv(sock, _recvbuf , sizeof(_recvbuf), 0);
        netmsg_DataHeader * re = (netmsg_DataHeader *)_recvbuf;
        _dl+=re->dataLength;;
        netmsg_LogoutR * logoutR = new netmsg_LogoutR;
        logoutR->result = 1;
        send(sock, (const char *)logoutR, sizeof(netmsg_LogoutR), 0);
    }

};
void w(){}
int main(){
    thread t1(w);
    server s;
    s.InitSocket();
    s.Bind();
    s.Listen();
    s.OnRun();
}