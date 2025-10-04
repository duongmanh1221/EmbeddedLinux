#include <sys/socket.h>  
#include <sys/un.h>      
#include <unistd.h>      
#include <string.h>      
#include <stdio.h>       
#include <stdlib.h>   

char * ip = "192.168.1.45";
int port = 8000;
char *respond = "Hi server";
struct sockaddr_un server_addr;


int main(){
    int client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, "/tmp/mysocket", sizeof(server_addr.sun_path) - 1);
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

