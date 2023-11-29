//
// Created by 18746 on 2023/11/25.
//

#ifndef C___WEBSERVER2_0_CELLLOG_H
#define C___WEBSERVER2_0_CELLLOG_H
#include"CELL.h"
#include"CELLTask.h"
#include<ctime>


class CELLlog{
/**
 *
 *
 *_logFile： 文件
 * _taskServer： 任务服务器，用来执行任务
 *
 * CELLlog()：任务服务器启动
 * ~CELLlog()：关闭服务器
 * static CELLlog & Instance()：执行单例模式返回当前对象
 * setLogPath(const char * logPath , const char * mode)：设置日志输出的位置
 * Info(const char * pStr)：日志输出 , 将要输出的内容交给_taskServer, 他会启动一个线程来执行这些任务
 * template<typename ...Args>
    static void Info(const char * pformat, Args ... args)：多参数的日志输出
 *
 *
 * */

/**
 *
 * int main() {
    CELLlog::Instance().setLogPath("log.txt","w");
    CELLlog::Info("this is a test!\n");
    CELLlog::Info("this is %d \n",8);

    return 0;
}
 *
 *
 *
 * */
private:
    FILE* _logFile = nullptr;
    CELLTaskServer _taskServer;
    CELLlog(){_taskServer.Start();}
    ~CELLlog(){
        _taskServer.Close();
        if(_logFile){
            Info("CELLLog fclose(_logFile)\n");
            fclose(_logFile);
            _logFile = nullptr;
        }
    }
public:
    static CELLlog & Instance(){
        static CELLlog sLog;
        return sLog;
    }
    void setLogPath(const char * logPath , const char * mode){
        if(_logFile){
            Info("CELLlog::setLogPath _logFile != nullptr\n");
            fclose(_logFile);
            _logFile = nullptr;
        }
        _logFile = fopen(logPath , mode);
        if(_logFile) Info("CELLLog::setLogPath success,<%s,%s>\n", logPath , mode);
        else Info("CELLLog::setLogPath failed,<%s,%s>\n", logPath , mode);

    }
    static int Info(const char * pStr){
        CELLlog * pLog = &Instance();
        pLog->_taskServer.addTask([=](){
            if(pLog->_logFile){
                time_point t = system_clock::now();
                time_t tNow = system_clock::to_time_t(t);
                tm * now  = gmtime(&tNow);
                fprintf(pLog->_logFile, "%s", "Info ");
                fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
                fprintf(pLog->_logFile, "%s", pStr);
                fflush(pLog->_logFile);
            }
        });
    }
    template<typename ...Args>
    static void Info(const char * pformat, Args ... args){
        CELLlog * pLog = &Instance();
        pLog->_taskServer.addTask([=]() {
            if (pLog->_logFile){
                auto t = system_clock::now();
                auto tNow = system_clock::to_time_t(t);
                std::tm* now = std::gmtime(&tNow);
                fprintf(pLog->_logFile, "%s", "Info ");
                fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
                fprintf(pLog->_logFile, pformat, args...);
                fflush(pLog->_logFile);
            }
            printf(pformat, args...);
        });
    }
};

#endif //C___WEBSERVER2_0_CELLLOG_H
