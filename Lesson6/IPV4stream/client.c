
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

char * ip = "192.168.1.45";
int port = 8000;
char *respond = "Hi server";
struct sockaddr_in server_addr;


int main(){
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
    perror("Invalid address");
    exit(1);
}

    connect(client_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    send(client_fd,respond,strlen(respond),0);
    char buff[1024];
    int n = read(client_fd,buff,sizeof(buff));
    buff[n] = '\0';
    printf("%s",buff);
    printf("\n");
    close(client_fd);
    return  0;
}

