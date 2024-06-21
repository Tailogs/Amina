#include "client.h"

void start_client(const char *address, int port) {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char message[1024];
    char username[256];

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock.\n");
        return;
    }

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);
    server_addr.sin_port = htons(port);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed.\n");
        closesocket(client_socket);
        WSACleanup();
        return;
    }

    printf("Connected to server.\n");

    const char *env_username = getenv("USERNAME");
    if (env_username == NULL) {
        printf("USERNAME environment variable is not set. Using 'Unknown'.\n");
        strcpy(username, "Unknown");
    } else {
        strcpy(username, env_username);
    }

    if (send(client_socket, username, strlen(username), 0) < 0) {
        printf("Send username failed.\n");
        closesocket(client_socket);
        WSACleanup();
        return;
    }

    HANDLE recv_thread = CreateThread(NULL, 0, receive_messages, (LPVOID)client_socket, 0, NULL);
    if (recv_thread == NULL) {
        printf("Failed to create receive thread.\n");
        closesocket(client_socket);
        WSACleanup();
        return;
    }

    while (1) {
        printf("(user) > ");
        fgets(message, sizeof(message), stdin);

        if (send(client_socket, message, strlen(message), 0) < 0) {
            printf("Send failed.\n");
            break;
        }
    }

    WaitForSingleObject(recv_thread, INFINITE);
    closesocket(client_socket);
    WSACleanup();
}

DWORD WINAPI receive_messages(LPVOID socket) {
    SOCKET client_socket = (SOCKET)socket;
    char buffer[1024];
    int bytes_received;

    while (1) {
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("\r%s(user) > ", buffer); // Отображение полученного сообщения выше строки ввода
            fflush(stdout); // Принудительная сброс буфера вывода

        } else if (bytes_received == 0) {
            printf("\nServer disconnected.\n");
            break;
        } else {
            printf("Receive failed.\n");
            break;
        }
    }

    return 0;
}
