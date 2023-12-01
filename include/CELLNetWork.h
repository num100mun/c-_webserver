//
// Created by 18746 on 2023/11/26.
//

#ifndef C___WEBSERVER2_0_CELLNETWORK_H
#define C___WEBSERVER2_0_CELLNETWORK_H
#include "CELL.h"
class CELLNetWork{

private:
    CELLNetWork(){
#ifdef _WIN32
        WORD ver = MAKEWORD(2,2);
        WSADATA dat;
        WSAStartup(ver, &dat);
#endif

#ifndef _WIN32
		signal(SIGPIPE, SIG_IGN);
#endif
    }

    ~CELLNetWork(){
#ifdef _WIN32
        WSACleanup();
#endif
    }
public:
    static void Init(){static  CELLNetWork obj;}
};
#endif //C___WEBSERVER2_0_CELLNETWORK_H
