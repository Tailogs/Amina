#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

#define MAX_CLIENTS 10

void start_server(int port);
void print_server_info(int port);
void broadcast_message(const wchar_t *wide_message, SOCKET sender_socket);
DWORD WINAPI handle_client(LPVOID client_socket);

#endif // SERVER_H
