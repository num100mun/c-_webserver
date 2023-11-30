//
// Created by 18746 on 2023/11/22.
//

#ifndef C___WEBSERVER2_0_CELLTASK_H
#define C___WEBSERVER2_0_CELLTASK_H
#include <thread>
#include <mutex>
#include <list>
#include <functional>
#include <atomic>
#include "CELLThread.h"
using namespace std;

class CELLTaskServer{
/**
 *
 * 启动一个线程来执行接收过来的任务
 *
 * CellTask：将void（）方法定义为CellTask
 * _tasks:任务列表
 * _tasksBuf：任务缓冲列表
 * _thread：线程任务
 * _serverId:所属服务器ID
 *
 *
 * addTask(CellTask task)：将task填入_tasksBuf
 * void Start()： 启动OnRun线程
 * void OnRun(CELLThread * pThread)：接收一个pThread， 判断_tasksBuf是否为空， 不空则加入_tasks 执行任务
 * void Close()：关闭线程
 * */


/**TEST
 *
 * void task(){
    printf("this is a task\n");
}
int main() {
    CELLThread myThread;
    CELLTaskServer cellTaskServer;
    typedef function<void()> t;
    vector<t> tasks;
    for( int i = 0; i< 10; i++) tasks.push_back([](){task();});
    for( int i = 0; i< 10; i++) cellTaskServer.addTask(tasks[i]);
    cellTaskServer.Start();
    cellTaskServer.Close();
    return 0;
}
 *
 *
 *
 * */
private:
    typedef function<void()> CellTask;
    list<CellTask> _tasks;
    list<CellTask> _tasksBuf;
    mutex _mutex;
    CELLThread _thread;
public:
    int _serverId = -1;
public:
    void addTask(CellTask task){
        lock_guard<mutex> lg(_mutex);
        _tasksBuf.push_back(task);
    }
    void Start(){
        _thread.Start(nullptr, [this](CELLThread* pThread){OnRun(pThread);});
    }
    void Close(){ _thread.Close(); }
protected:
    void OnRun(CELLThread * pThread){
        while(pThread->isRun()){
            if(!_tasksBuf.empty()){
                lock_guard<mutex> lg(_mutex);
                for(auto pt : _tasksBuf) _tasks.push_back(pt);
                _tasksBuf.clear();
            }
            else{
                chrono::milliseconds t(1);
                this_thread::sleep_for(t);
                continue;
            }
            for(auto pt : _tasks) pt();
            _tasks.clear();
        }
        for(auto pt : _tasksBuf) pt();
    }
};

#endif //C___WEBSERVER2_0_CELLTASK_H
