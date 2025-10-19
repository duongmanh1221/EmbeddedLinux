
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
   
#define MQ_MODE  (S_IRUSR | S_IWUSR)

#define MAX_MSG_SIZE 256

char mtext[MAX_MSG_SIZE]; 


int main(int argc, char *argv[]){
    if(argc != 3){
        printf("\nParameter pass to program must be 2");
        return -1;
    }


    struct  mq_attr attr = {
    .mq_flags = 0,
    .mq_maxmsg = 10,
    .mq_msgsize = MAX_MSG_SIZE,
    .mq_curmsgs = 0
};
    printf("\nCreate Quece");
    mqd_t mqid = mq_open("/mquece",O_WRONLY|O_NONBLOCK,MQ_MODE,&attr);
    if(mqid==-1){
        printf("\nopen failed error %d : %s",errno,strerror(errno));
        return -2;
    }
    int prio = atoi(argv[1]);
    char *task = argv[2];
    if(mq_getattr(mqid,&attr)!=0){
        printf("\nopen failed error %d : %s",errno,strerror(errno));
        return -3;
    }
    int l =strlen(task);
    memcpy(mtext,task,l+1);
    mq_send(mqid, mtext, l+1, prio);   
    sleep(1);
    mq_close(mqid);
    return 0;
 
}