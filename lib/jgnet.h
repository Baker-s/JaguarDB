#ifndef JGNET_H
#define JGNET_H

#pragma once

// std includes
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// Windows
#ifdef _WIN32
#pragma comment(lib,"ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#undef TEXT

#include <winsock2.h>
#include <ws2tcpip.h>
// Linux
#else

typedef int SOCKET;

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1;

#endif

typedef enum {
    Ok = (int8_t) 0,
    Err,
} Res;


class Sock {
    protected:
    u_int8_t SockInit(void);
    u_int8_t Init;
    std::string sIP;
    u_int16_t sPort;
    char* sSPort;
    struct sockaddr_in sSockAddr;

    void sInetPton(const char * host, struct sockaddr_in & saddr_in);
    void Clean(void);

    struct addrinfo * sAddrInfo;

    SOCKET sFD;
    SOCKET _conn;
    std::string sBuffer;
    std::string sMsg;

    public:
    Sock(const std::string, const u_int16_t);
    virtual ~Sock(void);
    u_int8_t Close(void);
};

class SockTcpServer : public Sock {
    public:
    SockTcpServer(const std::string ip, const u_int16_t port);
    void Connection(void);
    bool Send(void *buf, size_t len);
    bool Revc(void *buf, size_t len);
};

class SockTcpClient : public Sock {
    public:
    SockTcpClient(const std::string ip, const u_int16_t port);
    void Connect(void);
    bool Send(void *buf, size_t len);
    bool Revc(void *buf, size_t len);
};

#endif
