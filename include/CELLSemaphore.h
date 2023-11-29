//
// Created by 18746 on 2023/11/25.
//

#ifndef C___WEBSERVER2_0_CELLSEMAPHORE_H
#define C___WEBSERVER2_0_CELLSEMAPHORE_H

#include <chrono>
#include <thread>
#include <condition_variable>

using namespace std;
class CELLSemaphore{
/**
 * _mutex;锁
 * _cv：变量锁
 * _wait：等待计数
 * _wakeup：唤醒计数
 *
 * void wait()：当一个线程想要退出时 执行，等在Onrun运行结束后唤醒， 因为上了锁所以需要一个一个退出
 * void wakeup()： 唤醒wait
 * */
    /**
    * test
   CELLSemaphore semaphore;

   void close(){
       printf("prepare close!");
       semaphore.wait();
       printf("close success !");
   }
   void onrun(){
       printf("close is wait for 5 s!\n");
       this_thread::sleep_for(std::chrono::seconds(1));
       semaphore.wakeup();
       printf("weak up success!\n");
   }

   int main() {
       std::thread threads[2];
       threads[1] = thread(close);
       threads[0] = thread(onrun);
       for (int i = 0; i < 2; ++i) {threads[i].join();}
       return 0;
   }

    *
    *
    *
    * */
private:
    mutex _mutex;
    condition_variable _cv;
    int _wait = 0;
    int _wakeup = 0;
public:
    void wait(){
        unique_lock<mutex> lock(_mutex);
        if(--_wait < 0){
            _cv.wait(lock,[this]()->bool{return _wakeup>0;});
            --_wakeup;
        }
    }
    void wakeup(){
        lock_guard<mutex> lock(_mutex);
        if(++_wait <= 0){
            ++_wakeup;
            _cv.notify_all();
        }
    }
};

#endif //C___WEBSERVER2_0_CELLSEMAPHORE_H
