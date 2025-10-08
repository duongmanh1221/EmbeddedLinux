// util.c
#include "utils.h"
#include <stdio.h>

void printf_help() {
    printf("%-25s : %s\n", "1. help", "Display user interface options");
    printf("%-25s : %s\n", "2. myip", "Display IP address of this app");
    printf("%-25s : %s\n", "3. myport", "Display listening port of this app");
    printf("%-25s : %s\n", "4. connect <ip> <port>", "Connect to another app");
    printf("%-25s : %s\n", "5. list", "List all connections");
    printf("%-25s : %s\n", "6. terminate <id>", "Terminate a connection");
    printf("%-25s : %s\n", "7. send <id> <msg>", "Send message to connection");
    printf("%-25s : %s\n", "8. exit", "Close all connections & exit");
}
