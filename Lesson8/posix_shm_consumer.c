

#include <fcntl.h>       
#include <sys/mman.h>    
#include <sys/types.h>   
#include <sys/stat.h>    
#include <unistd.h>      
#include <stdio.h>       
#include <stdlib.h>      
#include <string.h>      

#include <sys/mman.h>    
#include <fcntl.h>       
#include <unistd.h>      
#include <stdio.h>       
#include <stdlib.h>      
#define FILE_NAME ("/lesson7")
#define MEM_SIZE (100)
typedef struct{
    char name[10];
    unsigned int id;
    unsigned long long price;
} Product;
int main()
{

    int shm_fd = shm_open(FILE_NAME,O_CREAT|O_RDWR,0777);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shm_fd, MEM_SIZE) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    char *data = (char *)mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    Product new_prd;
    strncpy(new_prd.name, "New", sizeof(new_prd.name) - 1);
    new_prd.name[sizeof(new_prd.name) - 1] = '\0';  // Đảm bảo null-terminated
    new_prd.id = 1234;
    new_prd.price = 12000;
    memcpy(data,&new_prd,sizeof(new_prd));
    munmap(data, MEM_SIZE);
    close(shm_fd);
    if (shm_unlink("/lesson7") == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
    return 0;
}