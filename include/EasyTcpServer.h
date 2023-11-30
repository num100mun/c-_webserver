#ifndef _EasyTcpServer_hpp_
#define _EasyTcpServer_hpp_

#include<vector>
#include<map>
#include<thread>
#include<mutex>
#include<atomic>
#include <functional>
#include <iostream>
#include"MessageHeader.h"
#include"CELLTimestamp.h"
#include "CELLClient.h"
#include "INetEvent.h"
#include "CELL.h"
//#include "../MemoryCtr/Alloc.h"
#include "CELLServer.h"
using namespace std;
/**
 * _sock:当前sercer的socket
 * vector<CELLServer*> _cellServers;
 * CELLTimestamp _tTime:每秒消息记时
 *atomic_int _recvCount： 接收数据量统计
 * atomic_int _msgCount： 收到消息计数
 * atomic_int _clientCount： 客户端计数
 *
 *
 * */
using namespace std;
class EasyTcpServer : public INetEvent{
private:
    SOCKET _sock;
    CELLThread _thread;
    vector<CellServer*> _cellServers;
    CELLTimestamp _tTime;
protected:
    std::atomic_int _recvCount;
    std::atomic_int _clientCount;
    atomic_int _msgCount;
public:
    EasyTcpServer(){
        _sock = INVALID_SOCKET;
        _recvCount = 0;
        _clientCount = 0;
        _msgCount = 0;
    }
    virtual ~EasyTcpServer(){Close();}
    SOCKET InitSocket(){
#ifdef _WIN32
        WORD ver = MAKEWORD(2, 2);
        WSADATA dat;
        WSAStartup(ver, &dat);
#endif
        if (INVALID_SOCKET != _sock){
            CELLlog::Info("warning, initSocket close old socket<%d>...\n", (int)_sock);
            Close();
        }
        _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (INVALID_SOCKET == _sock) CELLlog::Info("error, create socket failed...\n");
        else CELLlog::Info("create socket<%d> success...\n", (int)_sock);
        return _sock;
    }
    int Bind(const char* ip, unsigned short port){
        sockaddr_in _sin = {};
        _sin.sin_family = AF_INET;
        _sin.sin_port = htons(port);//host to net unsigned short
#ifdef _WIN32
        if (ip)_sin.sin_addr.S_un.S_addr = inet_addr(ip);
        else _sin.sin_addr.S_un.S_addr = INADDR_ANY;
#else
        if (ip) {
			_sin.sin_addr.s_addr = inet_addr(ip);
		}
		else {
			_sin.sin_addr.s_addr = INADDR_ANY;
		}
#endif
        int ret = bind(_sock, (sockaddr*)&_sin, sizeof(_sin));
        if (SOCKET_ERROR == ret) CELLlog::Info("error, bind port<%d> failed...\n", port);
        else CELLlog::Info("bind port<%d> success...\n", port);
        return ret;
    }
    int Listen(int n){
        int ret = listen(_sock, n);
        if (SOCKET_ERROR == ret) CELLlog::Info("error, listen socket<%d> failed...\n",_sock);
        else CELLlog::Info("listen port<%d> success...\n", _sock);
        return ret;
    }
    SOCKET Accept(){
        sockaddr_in clientAddr = {};
        int nAddrLen = sizeof(sockaddr_in);
        SOCKET cSock = INVALID_SOCKET;
#ifdef _WIN32
        cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
#else
        cSock = accept(_sock, (sockaddr*)&clientAddr, (socklen_t *)&nAddrLen);
#endif
        if (INVALID_SOCKET == cSock) CELLlog::Info("error, accept INVALID_SOCKET...\n");
        else addClientToCellServer(new CELLClient(cSock));
        return cSock;
    }
    void addClientToCellServer(CELLClient* pClient){
        auto pMinServer = _cellServers[0];
        for(auto pCellServer : _cellServers)
            if (pMinServer->getClientCount() > pCellServer->getClientCount()) pMinServer = pCellServer;
        pMinServer->addClient(pClient);
        OnNetJoin(pClient);
    }
    void Start(int nCellServer){
        for (int n = 0; n < nCellServer; n++){
            CellServer* ser = new CellServer(n+1);
            _cellServers.push_back(ser);
            ser->setEventObj(this);
            ser->Start();

        }
        _thread.Start(nullptr,
                      [this](CELLThread* pThread) {
                          OnRun(pThread);
                      });
    }
    void Close(){
        CELLlog::Info("EasyTcpServer.Close begin\n");
        _thread.Close();
        if (_sock != INVALID_SOCKET)
        {
            for (auto s : _cellServers)
            {
                delete s;
            }
            _cellServers.clear();
            //关闭套节字socket
#ifdef _WIN32
            closesocket(_sock);
#else
            close(_sock);
#endif
            _sock = INVALID_SOCKET;
        }
        CELLlog::Info("EasyTcpServer.Close end\n");
    }
    void OnRun(CELLThread* pThread){
        while (pThread->isRun()){
            time4msg();
            fd_set fdRead;
            FD_ZERO(&fdRead);
            FD_SET(_sock, &fdRead);
            timeval t = { 0,1};
            int ret = select(_sock + 1, &fdRead, 0, 0, &t); //
            if (ret < 0){
                CELLlog::Info("EasyTcpServer.OnRun select exit.\n");
                pThread->Exit();
                break;
            }
            if (FD_ISSET(_sock, &fdRead)){
                FD_CLR(_sock, &fdRead);
                Accept();
            }
        }
    }
    bool isRun(){return _sock != INVALID_SOCKET;}
    void time4msg(){
        auto t1 = _tTime.getElapsedSecond();
        if (t1 >= 1.0){
            CELLlog::Info("thread<%d>,time<%lf>,socket<%d>,clients<%d>,recv<%d>,msg<%d>\n", (int)_cellServers.size(), t1, _sock, (int)_clientCount, (int)(_recvCount / t1), (int)(_msgCount / t1));
            _recvCount = 0;
            _msgCount = 0;
            _tTime.update();
        }
    }
    virtual void OnNetJoin(CELLClient* pClient){_clientCount++;}
    virtual void OnNetLeave(CELLClient* pClient){_clientCount--;}
    virtual void OnNetMsg(CellServer* pCellServer, CELLClient* pClient, netmsg_DataHeader* header){_msgCount++;}
    virtual void OnNetRecv(CELLClient* pClient){_recvCount++;}
};

#endif // !_EasyTcpServer_hpp_
