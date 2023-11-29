//
// Created by 18746 on 2023/11/26.
//

#ifndef C___WEBSERVER2_0_CELLBUFFER_H
#define C___WEBSERVER2_0_CELLBUFFER_H
#include "CELL.h"
class CELLBuffer{
/**
 * 通过压入缓冲区数据， 来执行发送接收操作
 *
 * _pBuff:  第二缓冲区 发送缓冲区
 * _nLast:   缓冲区的数据尾部位置，已有数据长度
 * _nSize:  缓冲区总的空间大小，字节长度
 * _fullCount:缓冲区写满次数计数
 *
 * CELLBuffer(int nSize = 8192) : 构造函数默认 。缓冲区大小为8192
 * ~CELLBuffer():_pBuff不为空， 则对_pBuff进行释放
 * bool needWrite(): 是否继续读取
 * bool hasMsg(): 判断_nLast是否够netmsg_DataHeader， 和netmsg_DataHeader->dataLength
 * int read4socket(SOCKET): 接收数据：判断_nSize是否满了， 满了返回0，未满接受返回接受长度
 * int write2socket(SOCKET): 发送数据：存在数据则发送，返回发送数据长度
 * void pop(int nLen): 将_pBuff中的数据的前nLen,用后面的数据替换的，并且改版_nLast
 * bool push(const char * pData, int nLen) : 将pData中的数据放入_pBuff中， 如果大于_nSize将返回false
 * char * data(): 返回_pBuff
 *
 * */
 /**
  *
  * class YourTestClass {
public:
    static void runTest(SOCKET sock) {
        // Test Constructor
        CELLBuffer buffer(8192);
        netmsg_Login * log = new netmsg_Login;
        strcpy(log->userName, "hello") ;
        strcpy(log->PassWord, "World") ;
        assert(buffer.push((const char *)log, sizeof(netmsg_Login)));  // Should succeed
        // Test hasMsg method
        assert(buffer.hasMsg());  // No complete message yet
        delete log;
    }
};
  *
  *
  * */
private:
    char * _pBuff = nullptr;
    int _nLast = 0;
    int _nSize = 0;
    int _fullCount = 0;
public:
    CELLBuffer(int nSize = 8192){
        _nSize = nSize;
        _pBuff = new char[_nSize];
    }
    ~CELLBuffer(){
        if(_pBuff) {
            delete[] _pBuff;
            _pBuff = nullptr;
        }
    }
    bool push(const char * pData, int nLen){
        if(_nLast + nLen < _nSize){
            memcpy(_pBuff + _nLast, pData, nLen);
            _nLast += nLen;
            if(_nLast == SEND_BUFF_SIZE) ++_fullCount;
            return true;
        }
        else ++_fullCount;
        return false;
    }
    void pop(int nLen){
        int n = _nLast - nLen;
        if(n > 0) memcpy(_pBuff, _pBuff+nLen, n);
        _nLast = n;
        if(_fullCount > 0) --_fullCount;
    }
    int write2socket(SOCKET sockfd){
        int ret = 0;
        if(_nLast > 0 && INVALID_SOCKET!= sockfd){
            ret = send(sockfd, _pBuff, _nLast, 0);
            _nLast = 0;
            _fullCount = 0;
        }
        return ret;
    }
    int read4socket(SOCKET sockfd){
        if(_nSize - _nLast >0){
            char * szRecv = _pBuff + _nLast;
            int nLen = (int)recv(sockfd, szRecv, _nSize - _nLast, 0);
            if(nLen < 0) return nLen;
            _nLast += nLen;
            return nLen;
        }
        return 0;
    }
    bool hasMsg(){
        if(_nLast>= sizeof(netmsg_DataHeader)){
            netmsg_DataHeader * header = (netmsg_DataHeader *)_pBuff;
            return _nLast >= header->dataLength;
        }
        return false;
    }
    char * data(){return _pBuff;}
    bool needWrite(){return _nLast > 0;}
};
#endif //C___WEBSERVER2_0_CELLBUFFER_H
