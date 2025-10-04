
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
struct sockaddr_un client_addr;

int main(){
     int client_fd=socket(AF_UNIX, SOCK_DGRAM, 0);
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, "/tmp/mysocket", sizeof(server_addr.sun_path) - 1);
    socklen_t  l1 = sizeof(server_addr);
    
    
    memset(&client_addr,0,sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, "/tmp/mysocket_client", sizeof(client_addr.sun_path) - 1);
    socklen_t  l = sizeof(client_addr);
  unlink(client_addr.sun_path);

    bind(client_fd,(struct sockaddr*)&client_addr,sizeof(client_addr));
    

    sendto(client_fd,respond,strlen(respond),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
    char buff[1024];
    int n = recvfrom(client_fd,buff,sizeof(buff),0,(struct sockaddr*)&client_addr,&l);
    buff[n] = '\0';
    printf("%s",buff);
    printf("\n");
    close(client_fd);
    return  0;
}
