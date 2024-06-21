#ifndef PORTSCANNER_H
#define PORTSCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <pthread.h>
#endif

#define MAX_PORT 65535

typedef struct {
    const char *ip_address;
    int port;
} scan_params_t;

void scan_ports(const char *ip_address, int start_port, int end_port);

#endif // PORTSCANNER_H
