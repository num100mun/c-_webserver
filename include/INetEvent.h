//
// Created by 18746 on 2023/11/22.
//

#ifndef C___WEBSERVER2_0_INETEVENT_H
#define C___WEBSERVER2_0_INETEVENT_H

#include"CELL.h"
#include"CELLClient.h"
class CellServer;
class INetEvent{
public:
    virtual void OnNetJoin(CELLClient * pClient) = 0;
    virtual void OnNetLeave(CELLClient* pClient) = 0;
    virtual void OnNetMsg(CellServer* pCellServer, CELLClient* pClient, netmsg_DataHeader* header) = 0;
    virtual void OnNetRecv(CELLClient* pClient) = 0;

};


#endif //C___WEBSERVER2_0_INETEVENT_H
