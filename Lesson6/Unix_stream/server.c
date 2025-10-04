#include <sys/socket.h>  
#include <sys/un.h>      
#include <unistd.h>      
#include <string.h>      
#include <stdio.h>       
#include <stdlib.h>     


char * ip = "192.168.1.45";
int port = 8000;
char *respond = "OK client";
struct sockaddr_un server_addr;
struct sockaddr_un client_addr;
int main(void){
    int server_fd=socket(AF_UNIX, SOCK_STREAM, 0);
    if(server_fd<0)
    {
        perror("socket error");
        exit(1);
    }
    socklen_t client_len = sizeof(client_addr);
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, "/tmp/mysocket", sizeof(server_addr.sun_path) - 1);
    unlink("/tmp/mysocket"); 
    bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    listen(server_fd,2);
    int client_fd = accept(server_fd,&client_addr,&client_len);
    char buff[1024];
    int n = read(client_fd,buff,sizeof(buff));
    buff[n] = '\0';
    printf("%s",buff);
    printf("\n");
    send(client_fd,respond,strlen(respond),0);
    close(client_fd);
    close(server_fd);
    return 0;
}