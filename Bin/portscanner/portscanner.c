#include "portscanner.h"

#ifdef _WIN32
DWORD WINAPI scan_port(LPVOID param) {
#else
void *scan_port(void *param) {
#endif
    scan_params_t *params = (scan_params_t *)param;
    const char *ip_address = params->ip_address;
    int port = params->port;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 0;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip_address, &server_addr.sin_addr);

    int conn = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (conn < 0) {
        // Port is closed
    } else {
        printf("Port %d is open.\n", port);
    }

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif

    return 0;
}

void scan_ports(const char *ip_address, int start_port, int end_port) {
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return;
    }
#endif

    int num_ports = end_port - start_port + 1;
    scan_params_t *params = (scan_params_t *)malloc(num_ports * sizeof(scan_params_t));
    if (!params) {
        printf("Memory allocation failed\n");
        return;
    }

#ifdef _WIN32
    HANDLE *threads = (HANDLE *)malloc(num_ports * sizeof(HANDLE));
#else
    pthread_t *threads = (pthread_t *)malloc(num_ports * sizeof(pthread_t));
#endif

    if (!threads) {
        printf("Memory allocation failed\n");
        free(params);
        return;
    }

    for (int i = 0; i < num_ports; ++i) {
        params[i].ip_address = ip_address;
        params[i].port = start_port + i;

#ifdef _WIN32
        threads[i] = CreateThread(NULL, 0, scan_port, &params[i], 0, NULL);
#else
        pthread_create(&threads[i], NULL, scan_port, &params[i]);
#endif
    }

    for (int i = 0; i < num_ports; ++i) {
#ifdef _WIN32
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
#else
        pthread_join(threads[i], NULL);
#endif
    }

    free(params);
    free(threads);

#ifdef _WIN32
    WSACleanup();
#endif
}
