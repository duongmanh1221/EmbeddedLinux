
#include "my_config.h"
#include "server.h"



void *recv_handler(void *arg) {
    int sockfd = *((int *)arg);
    char buffer[1024];
    while (1) {
        int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) {
            printf("[-] Kết nối bị đóng từ server.\n");
            close(sockfd);
            pthread_exit(NULL);
        }
        buffer[n] = '\0';
        printf("\n[Server]: %s\n", buffer);
    }
    free(arg);
}

void *client_handler(void *arg) {
    int client_fd = *((int *)arg);
    free(arg);
    char respond[30];
    char buffer[1024];
    sprintf(respond, "%d", pid); 
    send(client_fd, respond, strlen(respond), 0);

    while (1) {
        int n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) {
            printf("[-] Kết nối bị đóng.\n");
            close(client_fd);
            pthread_exit(NULL);
        }
        buffer[n] = '\0';
        pid_t client_pid = atoi(buffer);
        printf("\n%s", buffer);

        for (int i = 0; i < num_conect; ++i) {
            if (listD[i].socket_fd == client_fd && listD[i].pid == -1) {
                listD[i].pid = client_pid;
                break;
            }
        }
        for (int i = 0; i < num_conect; ++i) {
            if (listD[i].socket_fd == client_fd && listD[i].port_t == -1) {
                listD[i].pid = client_pid;
                break;
            }
        }
    }
}

