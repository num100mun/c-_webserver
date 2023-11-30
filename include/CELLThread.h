//
// Created by 18746 on 2023/11/25.
//

#ifndef C___WEBSERVER2_0_CELLTHREAD_H
#define C___WEBSERVER2_0_CELLTHREAD_H

#include "CELLSemaphore.h"
#include <functional>
/**
 * 线程可以实现三个功能 创建，运行，销毁
 *
 * typedef std::function<void(CELLThread *)> EventCall： 声明了一个EventCall方法并没有实现
 * _onCreate：创建事件
 * _onRun：运行事件
 * _onDestory：销毁事件
 * _mutex：锁
 * _sem：等待
 *
 * OnWork()：如果事件存则运行
 * bool isRun()：判断是否在运行中
 * void Start(EventCall onCreate = nullptr, EventCall onRun = nullptr, EventCall onDestory = nullptr):创建事件开启新的线程并且执行OnWork
 * void Close()：如果OnWork没有唤醒值执行等待， 知道OnWork执行结束
 *  Exit()：设置_isRun 为false
 *
 * */
 /**TEST
  * std::atomic<int> run{1};

void onrun(CELLThread* cellthread) {
    printf("cellthread %d\n", cellthread->isRun());
    while (run) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        printf("onrun!\n");
    }
}

int main() {
    CELLThread myThread;
    myThread.Start(
            [](CELLThread* pThread) { std::cout << "Thread created" << std::endl; },
            [](CELLThread* pThread) { onrun(pThread); },
            [](CELLThread* pThread) { std::cout << "Thread destroyed" << std::endl; }
    );

    std::this_thread::sleep_for(std::chrono::seconds(2));
    myThread.Close();
    std::cout << "Main thread waiting for the CELLThread to finish..." << std::endl;

    // 等待子线程完成
    while (myThread.isRun()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Main thread exiting" << std::endl;

    return 0;
}
  *
  *
  *
  *
  * */
class CELLThread{
private:
    typedef std::function<void(CELLThread *)> EventCall;
    EventCall _onCreate = nullptr;
    EventCall _onRun = nullptr;
    EventCall _onDestory = nullptr;
    std::mutex _mutex;
    CELLSemaphore _sem;
    bool _isRun = false;
protected:
    void OnWork(){
        if(_onCreate) _onCreate(this);
        if(_onRun) _onRun(this);
        if(_onDestory) _onDestory(this);
        _sem.wakeup();
    }
public:
    bool isRun() {return _isRun;}
    void Start(EventCall onCreate = nullptr, EventCall onRun = nullptr, EventCall onDestory = nullptr){
        lock_guard<mutex> lock(_mutex);
        if(!_isRun){
            _isRun = true;
            if (onCreate) _onCreate = onCreate;
            if (onRun) _onRun = onRun;
            if (onDestory) _onDestory = onDestory;
            thread t(std::mem_fn(&CELLThread::OnWork), this);
            t.detach();
        }
    }
    void Close(){
        lock_guard<mutex> lock(_mutex);
        if(_isRun){
            _isRun = false;
            _sem.wait();
        }
    }
    void Exit(){
        lock_guard<mutex> lock(_mutex);
        if(_isRun) _isRun = false;
    }
};
#endif //C___WEBSERVER2_0_CELLTHREAD_H
