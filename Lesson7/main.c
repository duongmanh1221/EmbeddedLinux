#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(){
    int fd[2];
    if(pipe(fd)<0){
        perror("pipe thất bại");
        return 1;
    }
    pid_t p1,p2;

    p1 = fork();
    if(p1<0){
        perror("\nError");
        exit(1);
    }
    if(p1==0){
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        close(fd[1]);
        execlp("ls","ls","-l",(char *)NULL);

    }
    p2 = fork();
    if(p2<0){
        perror("\nError");
        exit(1);
    }
    if(p2==0){
        close(fd[1]);
        dup2(fd[0],STDIN_FILENO);
        close(fd[0]);
        execlp("wc","wc","-l",(char *)NULL);
    }
    close(fd[1]);
    close(fd[0]);
    wait(NULL);
    wait(NULL);
}
