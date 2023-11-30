//
// Created by 18746 on 2023/11/22.
//

#ifndef C___WEBSERVER2_0_CELLSERVER_H
#define C___WEBSERVER2_0_CELLSERVER_H
#include"CELL.h"
#include"INetEvent.h"
#include"CELLClient.h"
#include"CELLSemaphore.h"

#include<vector>
#include<map>
/**
 * void CheckTime():检查_clients中的元素是否超时
 * void ReadData(fd_set & fdRead):找到可用client，并使用RecvData 接收数据如果返回错误信息将移除数据
 * int RecvData(CELLClient* pClient): 处理网络数据，移除消息队列（缓冲区）最前的一条数据
 * virtual void OnNetMsg(CELLClient* pClient, netmsg_DataHeader* header)：执行OnNetMsg任务
 * void addClient(CELLClient* pClient):添加客户端到_clientsBuff
 * void Start()：_taskServer启动，_thread中onrun以及ondestory
 * size_t getClientCount():获取总的客户端数量 _clients+_clientsBuff内
 * */
class CellServer {
private:
    SOCKET _sock;
    std::map<SOCKET,CELLClient *> _clients;
    std::vector<CELLClient *> _clientsBuff;
    std::mutex _mutex;
    CELLThread _thread;
    INetEvent *_pNetEvent;
    CELLTaskServer _taskServer;
    fd_set _fdRead_bak;
    bool _clients_change;
    SOCKET _maxSock;
    int  _id = -1;
    time_t _oldTime = CELLTime::getNowInMilliSec();
private:
    void ClearClients(){
        for(auto iter : _clients) delete iter.second;
        _clients.clear();
        for(auto iter : _clientsBuff) delete iter;
        _clientsBuff.clear();
    }
public:
    void setEventObj(INetEvent *event) { _pNetEvent = event; }
    bool isRun() { return _sock != INVALID_SOCKET; }
    CellServer(int id) {
        _id = id;
        _pNetEvent = nullptr;
        _taskServer._serverId = id;
    }
    ~CellServer() {
        CELLlog::Info("CELLServer%d.~CELLServer exit begin\n", _id);
        Close();
        CELLlog::Info("CELLServer%d.~CELLServer exit end\n", _id);
    }
    void Close() {
        CELLlog::Info("CELLServer%d.Close begin\n", _id);
        _taskServer.Close();
        _thread.Close();
        CELLlog::Info("CELLServer%d.Close end\n", _id);
    }
    void clientBuff2clietns() {
        lock_guard<mutex> lg(_mutex);
        for (auto p: _clientsBuff) {
            _clients[p->sockfd()] = p;
            p->serverId = _id;
            if (_pNetEvent)
                _pNetEvent->OnNetJoin(p);
        }
        _clientsBuff.clear();
        _clients_change = true;
    }
    void getMaxsocket(fd_set &fdRead) {
        _maxSock = _clients.begin()->first;
        for (auto iter: _clients) {
            FD_SET (iter.first, &fdRead);
            if (_maxSock < iter.first) _maxSock = iter.first;
        }
        memcpy(&_fdRead_bak, &fdRead, sizeof(fd_set));
        _clients_change = false;
    }
    void OnRun(CELLThread * pThread) {
        _clients_change = true;
        fd_set fdRead;
        fd_set fdWrite;
        while (isRun()) {
            if (!_clientsBuff.empty()) clientBuff2clietns();
            if(_clients.empty()){
                this_thread::sleep_for(chrono::milliseconds(1));
                _oldTime = CELLTime::getNowInMilliSec();
                continue;
            }
            FD_ZERO(&fdRead);
            if (_clients_change) getMaxsocket(fdRead);
            else memcpy(&fdRead, &_fdRead_bak, sizeof(fd_set));
            memcpy(&fdWrite, &_fdRead_bak, sizeof(fd_set));
            timeval t{0,1};
            int ret = select(_maxSock + 1, &fdRead, nullptr, nullptr, &t);
            if (ret < 0) {
                CELLlog::Info("CELLServer%d.OnRun.select Error exit\n", _id);
                pThread->Exit();
                break;
            }
            ReadData(fdRead);
            WriteData(fdWrite);
            CheckTime();

        }
    }
    void CheckTime(){
        auto nowTime = CELLTime::getNowInMilliSec();
        auto dt = nowTime - _oldTime;
        _oldTime = nowTime;
        for(auto iter = _clients.begin(); iter != _clients.end();){
                if (iter->second->checkHeart(dt)) {
                    if (_pNetEvent) _pNetEvent->OnNetLeave(iter->second);
                    _clients_change = true;
                    delete iter->second;
                    auto iterold = iter;
                    iter++;
                    _clients.erase( iterold);
                    continue;
                }
                iter ++;
        }
    }
    void OnClientLeave(CELLClient * pClient){
        if(_pNetEvent) _pNetEvent->OnNetLeave(pClient);
        _clients_change = true;
        delete pClient;
    }
    void addSendTask(CELLClient* pClient, netmsg_DataHeader* header){
        _taskServer.addTask([pClient, header]() {
            pClient->SendData(header);
            delete header;
        });
    }
    void WriteData(fd_set& fdWrite) {
#ifdef _WIN32
        for(int n=0; n<fdWrite.fd_count;n++) {
            auto iter = _clients.find(fdWrite.fd_array[n]);
            if(iter != _clients.end()) {
                if(-1 == iter->second->SendDataReal()){
                    OnClientLeave(iter->second);
                    _clients.erase(iter);
                }
            }
        }

#else
    for (auto iter = _clients.begin(); iter != _clients.end(); )
		{
			if (FD_ISSET(iter->second->sockfd(), &fdWrite))
			{
				if (-1 == iter->second->SendDataReal())
				{
					OnClientLeave(iter->second);
					auto iterOld = iter;
					iter++;
					_clients.erase(iterOld);
					continue;
				}
			}
			iter++;
		}
#endif
    }
    void ReadData(fd_set & fdRead){
#ifdef _WIN32
        for (int n = 0; n < fdRead.fd_count; n++){
            auto iter = _clients.find(fdRead.fd_array[n]);
            if (iter != _clients.end()){
                if (-1 == RecvData(iter->second)){
                    OnClientLeave(iter->second);
                    _clients.erase(iter);
                }
            }
        }
#else
        for (auto iter = _clients.begin(); iter != _clients.end(); )
			{
				if (FD_ISSET(iter->second->sockfd(), &fdRead))
				{
					if (-1 == RecvData(iter->second))
					{
						OnClientLeave(iter->second);
						auto iterOld = iter;
						iter++;
						_clients.erase(iterOld);
						continue;
					}
				}
				iter++;
			}
#endif
    }
    int RecvData(CELLClient* pClient){
        int nLen = pClient->RecvData();
        if (nLen <= 0){return -1;}
        _pNetEvent->OnNetRecv(pClient);
        while(pClient->hasMsg()){
            OnNetMsg(pClient,pClient->front_msg());
            pClient->pop_front_msg();

        }
    }
    virtual void OnNetMsg(CELLClient* pClient, netmsg_DataHeader* header){_pNetEvent->OnNetMsg(this, pClient, header);}
    void addClient(CELLClient* pClient){
        std::lock_guard<std::mutex> lock(_mutex);
        _clientsBuff.push_back(pClient);
    }
    void Start(){
        _thread.Start(nullptr, [this](CELLThread * cellThread){ OnRun(cellThread);}, [this](CELLThread *cellThread){ClearClients();});
        CELLlog::Info("cellserver %d start...\n",_id);
        _taskServer.Start();
    }
    size_t getClientCount(){return _clients.size() + _clientsBuff.size();}




};
#endif //C___WEBSERVER2_0_CELLSERVER_H
