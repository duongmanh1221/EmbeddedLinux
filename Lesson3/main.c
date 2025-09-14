#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <sys/wait.h>

#define RUN_B1      (1)
#define RUN_B2      (2)
#define B3_ZOMBIE   (3)
#define B3_ORPHAN   (4)

// Ham chay bai 1. Truyen doi so 1 vao ham main
static void b1(){
    int rel_val;
    int a= 0;
    rel_val = fork();
    if(rel_val==0){
    printf("\nChild Process: \n");
    printf("With Pid is %d\n",getpid());
    exit(10);
    }
    else{
    printf("\nParrent Process: \n");
    printf("Parrenr Pid is %d,Child pid is %d\n",getpid(),rel_val);
    int status;
    pid_t id = wait(&status);       
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            printf("Parent: Child PID = %d exited normally with exit code = %d\n",
                   id, exit_code);
        } else {
            printf("Parent: Child PID = %d did not exit normally.\n", id);
        }
    }
}

// Ham chay bai 2. Truyen doi so 2 vao ham main
static void b2(){
    int rel_val;
    setenv("MY_COMMAND", "ls", 1); 
    rel_val = fork();
    if(rel_val<0){
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (rel_val>0){
        printf("\nthis is parrent process pid = %d\n",getpid());
        printf("Excute ls command\n");
        int status;
        pid_t id = wait(&status);
        printf("\nFinish Process\n");
    }
    else if (rel_val==0)
    {
        printf("\nThis is child process pid = %d\n",getpid());
    char * cmd = getenv("MY_COMMAND");
    if (cmd == NULL) {
        perror("getenv failed");
        exit(1);
    }
    execlp(cmd,cmd,(char *)NULL);
    perror("execlp failed");
    exit(EXIT_FAILURE);
    }
 

    /* Bao Cao*/
}

// Ham chay bai 3. Truyen doi so 3 hoac 4 vao ham main
static void b3( int option ){
    int rel_val;

    //Chuongw trinh tao tien trinh zombie
    if(option==B3_ZOMBIE){
        rel_val = fork();
        if(rel_val<0){
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        if(rel_val==0){
            printf("\nChildren pid ; %d",getpid());
            printf("\nParrent pid : %d",getppid());
            exit(EXIT_SUCCESS);
        }
        else if(rel_val>0){
            sleep(3);
            printf("\nparrent pid : %d",getpid());
        }
    }
    else if(option==B3_ORPHAN){
        rel_val = fork();
        if(rel_val<0){
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        if(rel_val==0){
            for (int i = 0; i < 10; i++) {
            pid_t ppid = getppid();  
            printf("Child: Parent PID = %d\n", ppid);
            sleep(1);
        }
        }
        else if(rel_val>0){
            printf("\nExit Parrent Process with PIA %d",getpid());
            sleep(5);
            exit(0);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Vui lòng truyền một đối số (1, 2 hoặc 3 or 4).\n");
        return 1;
    }
    int option = atoi(argv[1]);
    if(option>4 || option<0){
        printf("Vui lòng truyền một đối số (1, 2 hoặc 3 or 4 ).\n");
        return 1;
    }
    if(option==RUN_B1)
        b1();
    else if(option==RUN_B2)
        b2();
    else
        b3(option);
    return 0;
}