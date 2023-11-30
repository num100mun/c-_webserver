//
// Created by 18746 on 2023/11/22.
//

#ifndef C___WEBSERVER2_0_CELLCLIENT_H
#define C___WEBSERVER2_0_CELLCLIENT_H
#include"CELL.h"
#include"CELLBuffer.h"
#define CLIENT_HREAT_DEAD_TIME 60000
#define CLIENT_SEND_BUFF_TIME 2000
/**
 * 定义了一些客户端的基本操作，发送接收客户端的心跳检测，定时发送数据
 *
 *
 * CLIENT_HREAT_DEAD_TIME  对客户端进行心跳检测的频率
 * CLIENT_SEND_BUFF_TIME  发送数据的时间
 * _sockfd:当前client的 sock
 * serverId :所属服务器的id
 * _dtHeart; 心跳记时
 * _recvBuff: 第二接收缓冲区
 * _sendBuff : 发送缓冲区
 * _dtSend：上次发送数据的时间
 * _sendBuffFullCount：发送缓冲区写满次数记录
 *
 *
 * int RecvData(): 通过_recvBuff.read4socket接收数据
 * int SendDataReal()：_sendBuff.write2socket发送数据, 重置_dtSend
 *  int SendData(netmsg_DataHeader* header)： 通过_sendBuff.push将数据压入缓存中
 * bool hasMsg()： 通过_recvBuff.hasMsg() 判断是否有数据可以接收
 * netmsg_DataHeader* front_msg()： 通过_recvBuff.data()， 接收数据消息头
 * void pop_front_msg():如果hasMsg() 判断成功， 则通过_recvBuff.pop方法把数据压入_recvBuff种
 * bool needWrite():通过_sendBuff.needWrite()，读取数据
 * bool checkHeart(time_t dt):检查当前时间与_dtHearder相加是否超过CLIENT_HREAT_DEAD_TIME
 * SOCKET sockfd():返回socket
 * checkSend(time_t dt): 如果到达发送时间，则将缓冲区的数据发送过去
 * */
class CELLClient{
private:
    SOCKET _sockfd;
    CELLBuffer _recvBuff;
    CELLBuffer _sendBuff;
    time_t _dtHeart;
    time_t _dtSend;
public:
    int id = -1;
    int serverId = -1;
    int _sendBuffFullCount = 0;
public:
    CELLClient(SOCKET socketfd = INVALID_SOCKET) : _sendBuff(SEND_BUFF_SIZE), _recvBuff(RECV_BUFF_SIZE){
        static int n = 1;
        id = n++;
        _sockfd = socketfd;
        resetDTHeart();
        resetDTSend();
    }
    ~CELLClient(){
        CELLlog::Info("s = %d CELLClient %d.~CELLClient\n", serverId, id);
        if (INVALID_SOCKET != _sockfd)
        {
#ifdef _WIN32
            closesocket(_sockfd);
#else
            close(_sockfd);
#endif
            _sockfd = INVALID_SOCKET;
        }
    }
    SOCKET sockfd(){return _sockfd;}
    int RecvData(){return _recvBuff.read4socket(_sockfd);}
    bool hasMsg(){return _recvBuff.hasMsg();}
    netmsg_DataHeader* front_msg(){return (netmsg_DataHeader*)_recvBuff.data();}
    void pop_front_msg(){if(hasMsg())_recvBuff.pop(front_msg()->dataLength);}
    int SendData(netmsg_DataHeader* header){
        if (_sendBuff.push((const char*)header, header->dataLength)){return header->dataLength;}
        return SOCKET_ERROR;
    }
    bool needWrite(){return _sendBuff.needWrite();}
    int SendDataReal(){
        resetDTSend();
        return _sendBuff.write2socket(_sockfd);
    }
    void resetDTSend(){_dtSend = 0;}
    void resetDTHeart(){_dtHeart= 0;}
    bool checkHeart(time_t dt){
        _dtHeart += dt;
        if(_dtHeart >= CLIENT_HREAT_DEAD_TIME){
            CELLlog::Info("checkHeart dead:s = %d, time = %d\n",_sockfd,_dtHeart);
            return true;
        }
        return false;
    }
    bool checkSend(time_t dt){
        _dtSend += dt;
        if(_dtSend >= CLIENT_SEND_BUFF_TIME){
            CELLlog::Info("checkSend:s=%d,time=%d\n", _sockfd, _dtSend);
            SendDataReal();
            resetDTSend();
            return true;
        }
        return false;
    }

};
#endif //C___WEBSERVER2_0_CELLCLIENT_H
