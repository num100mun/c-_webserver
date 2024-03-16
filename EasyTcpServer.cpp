#include "include/EasyTcpServer.h"
#include "include/CELLlog.h"
class MyServer : public EasyTcpServer
{
public:
    virtual void OnNetJoin(CELLClient* pClient){EasyTcpServer::OnNetJoin(pClient);}
    virtual void OnNetLeave(CELLClient* pClient){EasyTcpServer::OnNetLeave(pClient);}
    virtual void OnNetMsg(CellServer* pServer, CELLClient* pClient, netmsg_DataHeader* header){
        EasyTcpServer::OnNetMsg(pServer, pClient, header);
        switch (header->cmd){
            case CMD_LOGIN:{
                pClient->resetDTHeart();
                netmsg_Login* login = (netmsg_Login*)header;
                netmsg_LoginR ret;
                if(SOCKET_ERROR == pClient->SendData(&ret)){CELLlog::Info("<Socket=%d> Send Full\n", pClient->sockfd());}
            }
                break;
            case CMD_LOGOUT:{
                netmsg_Logout* logout = (netmsg_Logout*)header;
                CELLlog::Info("recv <Socket=%d> msgType：CMD_LOGOUT, dataLen：%d,userName=%s \n", pClient->sockfd(), logout->dataLength, logout->userName);
                netmsg_LogoutR ret;
                pClient->SendData(&ret);
            }
                break;
            case CMD_C2S_HEART:
            {
                pClient->resetDTHeart();
                netmsg_s2c_Heart ret;
                pClient->SendData(&ret);
            }
            default:
            {
                CELLlog::Info("recv <socket=%d> undefine msgType,dataLen：%d\n", pClient->sockfd(), header->dataLength);
            }
                break;
        }
    }
private:

};
int main(){
    CELLlog::Instance().setLogPath("serverLog.txt","w");
    EasyTcpServer server;
    server.InitSocket();
//    server.Bind(nullptr, 4550);
//    server.Listen(64);
//    server.Start(4);
//    while (true){
//        char cmdBuf[256] = {};
//        scanf("%s", cmdBuf);
//        if (0 == strcmp(cmdBuf, "exit")){
//            server.Close();
//            break;
//        }
//        else {
//            CELLlog::Info("undefine cmd\n");
//        }
//    }
//    CELLlog::Info("exit.\n");
//    return 0;
}