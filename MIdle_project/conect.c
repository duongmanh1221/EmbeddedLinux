#include "conect.h"

#include "my_config.h"
#include "conect.h"
#include "server.h"


list_conect listD[5];
int num_conect = 0;
int port;
pid_t pid;

void *listen1_con(void *arg) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        pthread_exit(NULL);
    }

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_fd)) < 0) {
        perror("bind failed");
        close(server_fd);
        pthread_exit(NULL);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        pthread_exit(NULL);
    }

    printf("[+] Đang lắng nghe kết nối đến trên port %d...\n", port);

    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        if (num_conect < 5) {
            listD[num_conect].id = num_conect + 1;
            listD[num_conect].ip_t = strdup(inet_ntoa(client_addr.sin_addr));
            listD[num_conect].port_t = -1;
            listD[num_conect].socket_fd = client_fd;
            listD[num_conect].pid = -1;

            pthread_t t;
            int *arg_fd = malloc(sizeof(int));
            *arg_fd = client_fd;
            pthread_create(&t, NULL, client_handler, arg_fd);

            num_conect++;
        } else {
            printf("[-] Đã đạt tối đa số kết nối.\n");
            close(client_fd);
        }
    }

    close(server_fd);
    pthread_exit(NULL);
}

void conect(char *ip, int port_, pid_t p) {
    pthread_t t_main;
    conect_data *args = malloc(sizeof(conect_data));
    strcpy(args->ip, ip);
    args->port = port_;
    pthread_create(&t_main, NULL, add_new_conect, args);
}

void *add_new_conect(void *args_void) {
    conect_data *args = (conect_data *)args_void;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        pthread_exit(NULL);
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(args->port);

    if (inet_pton(AF_INET, args->ip, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        pthread_exit(NULL);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sockfd);
        pthread_exit(NULL);
    }

    char buffer[1024];
    int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0) {
        printf("\nClose connection");
        close(sockfd);
        pthread_exit(NULL);
    }
    buffer[n] = '\0';
    pid_t server_pid = atoi(buffer);
    printf("[+] Nhận PID server: %d\n", server_pid);

    char my_pid[30];
    sprintf(my_pid, "%d", pid);
    send(sockfd, my_pid, strlen(my_pid), 0);

    char my_port[30];
    sprintf(my_port, "%d", port); 
    send(sockfd, my_port, strlen(my_port), 0);

    pthread_t t_recv;
    int *arg_fd = malloc(sizeof(int));
    *arg_fd = sockfd;
    pthread_create(&t_recv, NULL, recv_handler, arg_fd);

    if (num_conect < 5) {
        listD[num_conect].id = num_conect + 1;
        listD[num_conect].ip_t = strdup(args->ip);
        listD[num_conect].port_t = args->port;
        listD[num_conect].socket_fd = sockfd;
        listD[num_conect].pid = server_pid;
        num_conect++;
    } else {
        printf("[-] Đã đạt số kết nối tối đa.\n");
        close(sockfd);
    }

    free(args);
    pthread_exit(NULL);
}

void conect_ter(int id){
    int index = -1;

    for (int i = 0; i < num_conect; ++i) {
        if (listD[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("[-] Không tìm thấy kết nối với ID %d.\n", id);
        return;
    }

    close(listD[index].socket_fd);
    free(listD[index].ip_t);

    for (int i = index; i < num_conect - 1; ++i) {
        listD[i] = listD[i + 1];
        listD[i].id = i + 1;
    }

    num_conect--;
    printf("[+] Đã ngắt kết nối với ID %d.\n", id);
}

void send_msg(int id, char *msg) {
    int index = -1;
    for (int i = 0; i < num_conect; ++i) {
        if (listD[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("[-] Không tìm thấy kết nối với ID %d.\n", id);
        return;
    }

    send(listD[index].socket_fd, msg, strlen(msg), 0);
}
