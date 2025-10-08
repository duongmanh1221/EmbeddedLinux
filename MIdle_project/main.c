// main.c
#include "my_config.h"
#include "conect.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);
    pid = getpid();

    printf("*************** Chat Application ********************\n\n");
    printf_help();

    pthread_t t_main;
    pthread_create(&t_main, NULL, listen1_con, NULL);

    char input[256];
    char *ip_conect;

    while (1) {
        printf("\nEnter your command: ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = 0;
            char *token = strtok(input, " ");

            if (!token) continue;

            if (strcmp(token, "help") == 0) {
                printf_help();
            } else if (strcmp(token, "myip") == 0) {
                printf("\n[+] IP: localhost (hoặc dùng ifconfig để kiểm tra)");
            } else if (strcmp(token, "myport") == 0) {
                printf("\n[+] Port: %d", port);
            } else if (strcmp(token, "connect") == 0) {
                ip_conect = strtok(NULL, " ");
                char *port_str = strtok(NULL, " ");
                if (ip_conect && port_str) {
                    conect(ip_conect, atoi(port_str), pid);
                } else {
                    printf("[-] Lệnh connect sai định dạng.\n");
                }
            } else if (strcmp(token, "list") == 0) {
                printf("\n%-5s %-20s %-10s %-10s\n", "ID", "IP", "PORT", "PID");
                for (int i = 0; i < num_conect; ++i) {
                    printf("%-5d %-20s %-10d %-10d\n", listD[i].id, listD[i].ip_t, listD[i].port_t, listD[i].pid);
                }
            } else if (strcmp(token, "exit") == 0) {
                for (int i = 0; i < num_conect; ++i) {
                    close(listD[i].socket_fd);
                    free(listD[i].ip_t);
                }
                printf("Đã thoát.\n");
                exit(0);
            } else if (strcmp(token, "terminate") == 0) {
                char *ter_id = strtok(NULL, " ");
                if(ter_id != NULL && atoi(ter_id) <= num_conect){
                    conect_ter(atoi(ter_id));
                }
                else{
                    printf("[-] Lệnh không hợp lệ. Gõ `help` để xem hướng dẫn.\n");
                }
            } else if (strcmp(token, "send") == 0) {
                char *id_str = strtok(NULL, " ");
                char *msg = strtok(NULL, "");
                if (id_str && msg) {
                    int id = atoi(id_str);
                    if(id <= num_conect && id > 0){
                        send_msg(id, msg);
                    } else {
                        printf("[-] ID không hợp lệ.\n");
                    }
                } else {
                    printf("[-] Lệnh không hợp lệ. Gõ `help` để xem hướng dẫn.\n");
                }
            } else {
                printf("[-] Lệnh không hợp lệ. Gõ `help` để xem hướng dẫn.\n");
            }
        }
    }

    return 0;
}
