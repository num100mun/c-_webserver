#include "include/CELL.h"
#include "include/EasyTcpClient.h"
#include <atomic>
bool g_bRun = true;
const int cCount = 400;
const int tCount = 4;
EasyTcpClient* client[cCount];
atomic_int sendCount(0);
atomic_int readyCount(0);

class myclient : public EasyTcpClient{
    virtual void OnNetMsg(netmsg_DataHeader * header){
        switch (header->cmd){
            case CMD_LOGIN_RESULT:{
                netmsg_LoginR* login = (netmsg_LoginR*)header;
                CELLlog::Info("<socket=%d> recv msgType:CMD_LOGIN_RESULT\n", (int)_pClient->sockfd());
                break;
            }
            case CMD_LOGOUT_RESULT:{
                netmsg_LogoutR* logout = (netmsg_LogoutR*)header;
                CELLlog::Info("<socket=%d> recv msgType:CMD_LOGOUT_RESULT\n", (int)_pClient->sockfd());
                break;
            }
            case CMD_NEW_USER_JOIN:{
                netmsg_NewUserJoin* userJoin = (netmsg_NewUserJoin*)header;
                CELLlog::Info("<socket=%d> recv msgType:CMD_NEW_USER_JOIN\n", (int)_pClient->sockfd());
                break;
            }
            case CMD_ERROR:{
                CELLlog::Info("<socket=%d> recv msgType:CMD_ERROR\n", (int)_pClient->sockfd());
                break;
            }
            default:{
                CELLlog::Info("error, <socket=%d> recv undefine msgType\n", (int)_pClient->sockfd());
            }
        }
    }
};
void cmdThread(){
    while (true){
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        if (0 == strcmp(cmdBuf, "exit")){
            g_bRun = false;
            CELLlog::Info("退出cmdThread线程\n");
            break;
        }
        else CELLlog::Info("不支持的命令。\n");
    }
}
void recvThread(int begin, int end){
    while(g_bRun)
    for (int n = begin; n < end; n++)
        client[n]->OnRun();
}
void sendThread(int id){
    CELLlog::Info("thread<%d>,start\n", id);
    int c = cCount / tCount;
    int begin = (id - 1)*c;
    int end = id*c;
    for (int n = begin; n < end; n++) client[n] = new myclient();

    for (int n = begin; n < end; n++)
        client[n]->Connect("127.0.0.1", 4550);
    CELLlog::Info("thread<%d>,Connect<begin=%d, end=%d>\n", id, begin, end);
    readyCount++;
    while (readyCount < tCount){
        std::chrono::milliseconds t(10);
        std::this_thread::sleep_for(t);
    }
    std::thread t1(recvThread, begin, end);
    t1.detach();

    netmsg_Login login[1];
    for (int n = 0; n < 1; n++){
        strcpy(login[n].userName, "lyd");
        strcpy(login[n].PassWord, "lydmm");
    }
    const int nLen = sizeof(login);

    while (g_bRun){
        for (int n = begin; n < end; n++)
            if (SOCKET_ERROR != client[n]->SendData(login))  sendCount++;
//        std::chrono::milliseconds t(99);
//        std::this_thread::sleep_for(t);
    }

    for (int n = begin; n < end; n++){
        client[n]->Close();
        delete client[n];
    }
    CELLlog::Info("thread<%d>,exit\n", id);
}
int main(){
    CELLlog::Instance().setLogPath("clientLog.txt", "w");
    thread t1(cmdThread);
    t1.detach();
    for (int n = 0; n < tCount; n++){
        std::thread t1(sendThread,n+1);
        t1.detach();
    }
    CELLTimestamp tTime;

    while (g_bRun){
        auto t = tTime.getElapsedSecond();
        if (t >= 1.0){
            CELLlog::Info("thread<%d>,clients<%d>,time<%lf>,send<%d>\n",tCount, cCount,t,(int)(sendCount/ t));
            sendCount = 0;
            tTime.update();
        }
        std::chrono::milliseconds ts(1);
        std::this_thread::sleep_for(ts);
    }

    CELLlog::Info("已退出。\n");
    return 0;
}