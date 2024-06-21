#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")

void start_client(const char *address, int port);
DWORD WINAPI receive_messages(LPVOID socket);

#endif // CLIENT_H
