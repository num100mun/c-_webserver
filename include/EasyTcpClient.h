//
// Created by 18746 on 2023/11/25.
//

#ifndef C___WEBSERVER2_0_EASYTCPCLIENT_H
#define C___WEBSERVER2_0_EASYTCPCLIENT_H
#include "CELLClient.h"
#include "CELL.h"
#include "CELLNetWork.h"
#include "CELLSemaphore.h"

class EasyTcpClient{
    /**
     * _pClient:当前client
     * _isCounect：是否链接
     *
     *
     * InitSock,Connect,isRun
     * Close():关闭_pClient,_isCoounnect为false
     *RecvData(SOCKET cSock):接收数据后弹出数据
     *
     * */
protected:
    CELLClient * _pClient = nullptr;
    bool _isConnect = false;
    CELLSemaphore _se;
public:
    EasyTcpClient(){_isConnect = false;}
    virtual ~EasyTcpClient(){Close();}
    void InitSock(){
        CELLNetWork::Init();
        if (_pClient){
            CELLlog::Info("warning, initSocket close old socket<%d>...\n", (int)_pClient->sockfd());
            Close();
        }
        SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(INVALID_SOCKET == sock) CELLlog::Info("error, create socket failed...\n");
        else _pClient = new CELLClient(sock);
    }
    int Connect(const char *ip , unsigned  short port){
        if(!_pClient) InitSock();
        sockaddr_in _sin = {};
        _sin.sin_family = AF_INET;
        _sin.sin_port = htons(port);
#ifdef _WIN32
        _sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
        _sin.sin_addr.s_addr = inet_addr(ip);
#endif
        int ret = connect(_pClient->sockfd(), (sockaddr*)&_sin, sizeof(sockaddr_in));
        if (SOCKET_ERROR == ret){CELLlog::Info("<socket=%d> connect <%s:%d> failed...\n", (int)_pClient->sockfd(), ip, port);}
        else _isConnect = true;
        return ret;
    }
    void Close(){
        if(_pClient){
            delete _pClient;
            _pClient = nullptr;
        }
        _isConnect = false;
        CELLlog::Info("socket %d close",_pClient->sockfd());
    }
    bool isRun(){return _pClient&&_isConnect;}
    bool OnRun(){
        if(isRun()){
            SOCKET _sock = _pClient->sockfd();
            fd_set fdRead;
            FD_ZERO(&fdRead);
            FD_SET(_sock, &fdRead);
            fd_set fdWrite;
            FD_ZERO(&fdWrite);
            timeval t = { 1,1 };
            int ret = 0;
            if (_pClient->needWrite()){
                FD_SET(_sock, &fdWrite);
                ret = select(_sock + 1, &fdRead, &fdWrite, nullptr, &t);
            }else ret = select(_sock + 1, &fdRead, nullptr, nullptr, &t);
            if (ret < 0){
                CELLlog::Info("error,<socket=%d>OnRun.select exit\n", (int)_sock);
                Close();
                return false;
            }
            if (FD_ISSET(_sock, &fdRead)){
                if (-1 == RecvData(_sock)){
                    CELLlog::Info("error,<socket=%d>OnRun.select RecvData exit\n", (int)_sock);
                    Close();
                    return false;
                }
            }
            if (FD_ISSET(_sock, &fdWrite)){
                if (-1 == _pClient->SendDataReal()){
                    CELLlog::Info("error,<socket=%d>OnRun.select SendDataReal exit\n", (int)_sock);
                    Close();
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    int RecvData(SOCKET cSock){
        int nLen = _pClient->RecvData();
        if( nLen > 0){
            while(_pClient->hasMsg()){
                OnNetMsg((_pClient->front_msg()));
                _pClient->pop_front_msg();
            }
        }
        return nLen;
    }
    virtual void OnNetMsg(netmsg_DataHeader * header) = 0;
    int SendData(netmsg_DataHeader * header){ return _pClient->SendData(header);}
};
#endif //C___WEBSERVER2_0_EASYTCPCLIENT_H
