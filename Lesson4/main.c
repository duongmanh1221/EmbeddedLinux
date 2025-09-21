#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define RUN_B1      (1)
#define RUN_B2      (2)
#define RUN_B3      (3)
#define RUN_B4      (4)


#define MAX_COUNTER (10000)

#define READER_THREAD_NUM  (5)
#define WRITER_THREAD_NUM  (2)

/*
    All static funtion for B1
*/

static void *func_for_thread1_b1(void* arg){
    printf("\nThis is thread 1 with ID : %llu",(unsigned long long)pthread_self());
}

static void *func_for_thread2_b1(void* arg){
    printf("\nThis is thread 2 with ID : %llu",(unsigned long long)pthread_self());
}

// Ham chay bai 1. Truyen doi so 1 vao ham main
static void b1(void){
    pthread_t t1, t2;
    pthread_t t_main;
    printf("\nThis is thread main with ID : %llu",(unsigned long long)pthread_self());
    pthread_create(&t1,NULL,func_for_thread1_b1,NULL);
    pthread_create(&t2,NULL,func_for_thread2_b1,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("\nEnd");
 
}

/*
    Static function for b2
*/

unsigned long long counter = 0;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;


static void *func_for_thread1_b2(void* arg){
    pthread_detach(pthread_self());
    
    while(counter<=MAX_COUNTER)
    {
        pthread_mutex_lock(&lock1);
        printf("\nCurrent value counter in thread 1  : %d",counter++);
        pthread_mutex_unlock(&lock1);
    }
    
}

static void *func_for_thread2_b2(void* arg){
    pthread_detach(pthread_self());
    
    while(counter<=MAX_COUNTER)
    {
        pthread_mutex_lock(&lock1);
        printf("\nCurrent value counter in thread 2  : %d",counter++);
        pthread_mutex_unlock(&lock1);
    }
    
}

static void *func_for_thread3_b2(void* arg){
    pthread_detach(pthread_self());
    
    while(counter<=MAX_COUNTER)
    {
        pthread_mutex_lock(&lock1);
        printf("\nCurrent value counter in thread 3  : %d",counter++);
        pthread_mutex_unlock(&lock1);
    }
    
}

// Ham chay bai 2. Truyen doi so 2 vao ham main
static void b2(void){
    pthread_t t1, t2;
    pthread_t t3;

    pthread_create(&t1,NULL,func_for_thread1_b2,NULL);
    pthread_create(&t2,NULL,func_for_thread2_b2,NULL);
    pthread_create(&t3,NULL,func_for_thread3_b2,NULL);
    sleep(5);
    printf("\nEnd");

}

/*
    static function for b3
*/

pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t condition1 = PTHREAD_COND_INITIALIZER;

int data_ready =0;
int data;

static void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&lock2);

        // Tạo dữ liệu
        data = rand() % 100;
        data_ready = 1;
        printf("Producer: tạo data = %d\n", data);

        pthread_cond_signal(&condition1); // Báo Consumer
        pthread_mutex_unlock(&lock2);

        sleep(1); // mô phỏng thời gian sản xuất
    }
    return NULL;
}

static void* consumer(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&lock2);

        while (!data_ready) {
            pthread_cond_wait(&condition1, &lock2);
        }

        printf("Consumer: đọc data = %d\n", data);
        data_ready = 0;

        pthread_mutex_unlock(&lock2);
    }
    return NULL;
}

// Ham chay bai 3. Truyen doi so 3 hoac 4 vao ham main
static void b3( void){
    pthread_t prod, cons;

    pthread_mutex_init(&lock2, NULL);
    pthread_cond_init(&condition1, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&lock2);
    pthread_cond_destroy(&condition1);
    return 0;
}

/*
    Static function for b4
*/

pthread_rwlock_t file_lock;
int share_source;

static void *read4(void* arg ){
    pthread_rwlock_rdlock(&file_lock);
    int value = share_source;
    pthread_rwlock_unlock(&file_lock);
    printf("\n Read : %d",value);
    usleep(50000);
}

static void *write4(void* arg ){
    pthread_rwlock_wrlock(&file_lock);
    share_source++;
    pthread_rwlock_unlock(&file_lock);
    usleep(60000);
}

// Ham chay bai 4. Truyen doi so 3 hoac 4 vao ham main
static void b4( void ){
    pthread_t Write[WRITER_THREAD_NUM];
    pthread_t Read[READER_THREAD_NUM];

    int i;

    pthread_rwlock_init(&file_lock, NULL);

    for(i=0;i<WRITER_THREAD_NUM;i++){
        pthread_create(&Write,NULL,write4,NULL);
    }

    for(i=0;i<READER_THREAD_NUM;i++){
        pthread_create(&Read,NULL,read4,NULL);
    }

    for (i = 0; i < WRITER_THREAD_NUM; i++) {
        pthread_join(Write[i], NULL);
    }
    for (i = 0; i < READER_THREAD_NUM; i++) {
        pthread_join(Read[i], NULL);
    }
    pthread_rwlock_destroy(&file_lock);

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
    else if(option==RUN_B3)
        b3();
    else 
        b4();
    return 0;
}

