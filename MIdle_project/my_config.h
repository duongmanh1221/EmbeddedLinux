#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct list_conect {
    int id;
    char *ip_t;
    int port_t;
    int socket_fd;
    pid_t pid;
} list_conect;

typedef struct conect_data {
    char ip[64];
    int port;
} conect_data;

extern list_conect listD[5];
extern int num_conect;
extern int port;
extern pid_t pid;

#endif