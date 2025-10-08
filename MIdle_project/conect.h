
#ifndef CONNECTION_H
#define CONNECTION_H
#include "my_config.h"
void *listen1_con(void *arg);
void conect(char *ip, int port, pid_t p);
void *add_new_conect(void *args_void);
void conect_ter(int id);
void send_msg(int id, char *msg);

#endif
