#include "server.h"

typedef struct {
    SOCKET socket;
    wchar_t username[256];
} Client;

Client clients[MAX_CLIENTS];
int num_clients = 0;
CRITICAL_SECTION clients_lock;

void broadcast_message(const wchar_t *wide_message, SOCKET sender_socket) {
    EnterCriticalSection(&clients_lock);
    char send_buffer[1024];
    WideCharToMultiByte(CP_UTF8, 0, wide_message, -1, send_buffer, sizeof(send_buffer), NULL, NULL);

    for (int i = 0; i < num_clients; ++i) {
        if (clients[i].socket != sender_socket) {
            send(clients[i].socket, send_buffer, strlen(send_buffer), 0);
        }
    }
    LeaveCriticalSection(&clients_lock);
}

void print_server_info(int port) {
    WSADATA wsaData;
    DWORD dwRetval;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Winsock initialization failed\n");
        return;
    }

    ULONG outBufLen = 15000;
    IP_ADAPTER_ADDRESSES *pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);

    if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAddresses);
        pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);
    }

    if ((dwRetval = GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen)) == NO_ERROR) {
        IP_ADAPTER_ADDRESSES *pCurrAddresses = pAddresses;
        while (pCurrAddresses) {
            printf("Adapter name: %S\n", pCurrAddresses->FriendlyName);
            IP_ADAPTER_UNICAST_ADDRESS *pUnicast = pCurrAddresses->FirstUnicastAddress;
            while (pUnicast) {
                char addressBuffer[INET6_ADDRSTRLEN];
                void *addrPtr = NULL;

                if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
                    addrPtr = &((struct sockaddr_in *)pUnicast->Address.lpSockaddr)->sin_addr;
                    inet_ntop(AF_INET, addrPtr, addressBuffer, sizeof(addressBuffer));
                } else if (pUnicast->Address.lpSockaddr->sa_family == AF_INET6) {
                    addrPtr = &((struct sockaddr_in6 *)pUnicast->Address.lpSockaddr)->sin6_addr;
                    inet_ntop(AF_INET6, addrPtr, addressBuffer, sizeof(addressBuffer));
                }

                if (addrPtr) {
                    printf("\tIP Address: %s\n", addressBuffer);
                }

                pUnicast = pUnicast->Next;
            }
            pCurrAddresses = pCurrAddresses->Next;
        }
    } else {
        printf("Call to GetAdaptersAddresses failed with error: %d\n", dwRetval);
    }

    if (pAddresses) {
        free(pAddresses);
    }

    WSACleanup();
    printf("Server port: %d\n", port);
}

void start_server(int port) {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock.\n");
        return;
    }

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    if (listen(server_socket, 5) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    InitializeCriticalSection(&clients_lock);
    print_server_info(port);

    while (1) {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) == INVALID_SOCKET) {
            printf("Accept failed.\n");
            closesocket(server_socket);
            WSACleanup();
            return;
        }

        EnterCriticalSection(&clients_lock);
        if (num_clients < MAX_CLIENTS) {
            clients[num_clients].socket = client_socket;

            // Прием имени пользователя от клиента
            char recv_buffer[1024];
            int bytes_received = recv(client_socket, recv_buffer, sizeof(recv_buffer) - 1, 0);
            if (bytes_received > 0) {
                recv_buffer[bytes_received] = '\0';
                MultiByteToWideChar(CP_UTF8, 0, recv_buffer, -1, clients[num_clients].username, sizeof(clients[num_clients].username) / sizeof(clients[num_clients].username[0]));
            } else {
                printf("Receive username failed.\n");
                closesocket(client_socket);
                LeaveCriticalSection(&clients_lock);
                continue;
            }

            num_clients++;

            printf("Client connected from %s:%d with username: %S\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), clients[num_clients - 1].username);

            HANDLE client_thread = CreateThread(NULL, 0, handle_client, (LPVOID)client_socket, 0, NULL);
            if (client_thread == NULL) {
                printf("Failed to create thread for client.\n");
                closesocket(client_socket);
                num_clients--;
            }
        } else {
            printf("Too many clients. Connection rejected.\n");
            closesocket(client_socket);
        }
        LeaveCriticalSection(&clients_lock);
    }

    DeleteCriticalSection(&clients_lock);
    closesocket(server_socket);
    WSACleanup();
}

DWORD WINAPI handle_client(LPVOID socket) {
    SOCKET client_socket = (SOCKET)socket;
    char buffer[1024];
    wchar_t wide_buffer[1024];
    int bytes_received;

    bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        closesocket(client_socket);
        return 1;
    }
    buffer[bytes_received] = '\0';

    // Получение имени пользователя
    wchar_t username[256];
    MultiByteToWideChar(CP_UTF8, 0, buffer, -1, username, sizeof(username) / sizeof(username[0]));

    EnterCriticalSection(&clients_lock);
    for (int i = 0; i < num_clients; ++i) {
        if (clients[i].socket == client_socket) {
            wcscpy_s(clients[i].username, sizeof(clients[i].username) / sizeof(clients[i].username[0]), username);
            break;
        }
    }
    LeaveCriticalSection(&clients_lock);

    while (1) {
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            //printf("Received from %S: %s\n", username, buffer);

            wchar_t message_with_username[1280];
            swprintf(message_with_username, sizeof(message_with_username) / sizeof(wchar_t), L"%s: %S", username, buffer);

            EnterCriticalSection(&clients_lock);
            broadcast_message(message_with_username, client_socket);
            LeaveCriticalSection(&clients_lock);
        } else if (bytes_received == 0) {
            printf("Client %S disconnected.\n", username);
            break;
        } else {
            printf("Receive failed.\n");
            break;
        }
    }

    EnterCriticalSection(&clients_lock);
    for (int i = 0; i < num_clients; ++i) {
        if (clients[i].socket == client_socket) {
            memmove(clients + i, clients + i + 1, (num_clients - i - 1) * sizeof(Client));
            num_clients--;
            break;
        }
    }
    LeaveCriticalSection(&clients_lock);

    closesocket(client_socket);
    return 0;
}