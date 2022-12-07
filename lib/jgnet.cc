#include "jgnet.h"

Sock::Sock(const std::string ip, const u_int16_t port) {
    Init = SockInit();
    if (Init != 0) {
        printf("Init error\n");
        exit(1);
    }
}

Sock::~Sock(void) {
    Clean();
}

void Sock::Clean(void) {
    #ifdef _WIN32
        WSACleanup();
    #endif
}

void Sock::sInetPton(const char * host, struct sockaddr_in & saddr_in) {
    #ifdef _WIN32
        WCHAR wsz[64];
        swprintf_s(wsz, L"%S", host);
        InetPton(AF_INET, wsz,   &(saddr_in.sin_addr.s_addr));
    #else
        inet_aton(host, (struct in_addr*)&saddr_in.sin_addr.s_addr);
    #endif
}


bool SockTcpServer::Send(void *buf, size_t len) {
    return (size_t)send(_conn, (const char *)buf, len, 0) == len;
}

bool SockTcpServer::Revc(void *buf, size_t len) {
    return (size_t)recv(_conn, (char *)buf, len, 0) == len;
}

bool SockTcpClient::Send(void *buf, size_t len) {
    return (size_t)send(sFD, (const char *)buf, len, 0) == len;
}

bool SockTcpClient::Revc(void *buf, size_t len) {
    return (size_t)recv(sFD, (char *)buf, len, 0) == len;
}


u_int8_t Sock::SockInit(void) {
    #ifdef _WIN32
        WSADATA wsa_data;
        return WSAStartup(MAKEWORD(1,1), &wsa_data);
    #else
        return 0;
    #endif
}

u_int8_t Sock::Close(void) {
    int status = 0;

    #ifdef _WIN32
        status = shutdown(sFD, SD_BOTH);
        if (status == 0) { status = closesocket(sFD); }
    #else
        status = shutdown(sFD, SHUT_RDWR);
        if (status == 0) { status = close(sFD); }
    #endif

  return status;
}

SockTcpServer::SockTcpServer(const std::string ip, const u_int16_t port) : Sock(ip, port) {
    sSockAddr.sin_family = AF_INET;
    sSockAddr.sin_port = htons(port);

    sInetPton(ip.c_str(), sSockAddr);

    sFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sFD == INVALID_SOCKET) {
        Clean();
        return;
    }

    if (0 > bind(sFD, (struct sockaddr*)&sSockAddr, sizeof(sSockAddr))) {
       printf("Bind error\n");
       exit(1);
    }
}

void SockTcpServer::Connection() {
    if (listen(sFD, 1)  == -1) {
        printf("Failed listen");
        return;
    }

    printf("Waiting for client to connect on %s:%s ... ", sIP.c_str(), sSPort);
    fflush(stdout);
    _conn = accept(sFD, (struct sockaddr *)NULL, NULL);
    if (_conn == -1) {
        printf("Failed accept");
        return;
    }
    printf("connected\n");
    fflush(stdout);
}

SockTcpClient::SockTcpClient(const std::string ip, const u_int16_t port) : Sock(ip, port) {
    sSockAddr.sin_family = AF_INET;
    sSockAddr.sin_port = htons(port);

    sInetPton(ip.c_str(), sSockAddr);

    sFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sFD == INVALID_SOCKET) {
        Clean();
        return;
    }
}

void SockTcpClient::Connect() {
    if (connect(sFD, (struct sockaddr *)&sSockAddr, sizeof(sSockAddr)) == -1) {
        Close();
        printf("Faild Client Connect\n");
        return;
    }
}
