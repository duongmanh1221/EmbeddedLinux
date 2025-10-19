#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>       
#include <sys/mman.h>    
#include <sys/types.h>   
#include <sys/stat.h>    
#include <unistd.h>      
#include <stdio.h>       
#include <stdlib.h>      
#include <string.h>      
#include <errno.h>
#include <signal.h>
   
#define MQ_MODE  (S_IRUSR | S_IWUSR)

#define MAX_MSG_SIZE 256

             
char mtext[MAX_MSG_SIZE]; 

 mqd_t mqid ;
void signint_handler(int signo){
    mq_close(mqid);
    mq_unlink("/mquece");
    exit(0);
}

int main(){
    struct  mq_attr attr = {
    .mq_flags = 0,
    .mq_maxmsg = 10,
    .mq_msgsize = MAX_MSG_SIZE,
    .mq_curmsgs = 0
};
    printf("\nCreate Quece");
    mqid = mq_open("/mquece",O_RDONLY|O_CREAT|O_NONBLOCK,MQ_MODE,&attr);
    if(mqid==-1){
        printf("\nopen failed error %d : %s",errno,strerror(errno));
        return -2;
    }

    signal(SIGINT, signint_handler);
    if(mq_getattr(mqid,&attr)!=0){
        printf("\nopen failed error %d : %s",errno,strerror(errno));
        return -3;
    }
    printf("\n");

    printf("Queue Attributes:\n");
    printf("  mq_flags   : %ld\n", attr.mq_flags);
    printf("  mq_maxmsg  : %ld\n", attr.mq_maxmsg);
    printf("  mq_msgsize : %ld\n", attr.mq_msgsize);
    printf("  mq_curmsgs : %ld\n", attr.mq_curmsgs);
    unsigned int prio;
    while (1)
    {
        ssize_t bytes=mq_receive(mqid,&mtext,MAX_MSG_SIZE,&prio);
        if(bytes>0)
            printf("\nTask: %s, prio : %u",mtext,prio);
        fflush(stdout);
        /* code */
    }

    

    
    mq_close(mqid);
    return 0;
 
}