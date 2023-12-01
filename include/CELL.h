//
// Created by 18746 on 2023/11/22.
//

#ifndef C___WEBSERVER2_0_CELL_H
#define C___WEBSERVER2_0_CELL_H
#include "MessageHeader.h"
#include "CELLTimestamp.h"
#include "CELLTask.h"
#include "CELLlog.h"
#ifdef WIN32
#define FD_SETSIZE 256
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<windows.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<signal.h>

#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#endif
#ifndef RECV_BUFF_SIZE
#define RECV_BUFF_SIZE 10240
#define SEND_BUFF_SIZE RECV_BUFF_SIZE
#endif
#endif //C___WEBSERVER2_0_CELL_H
