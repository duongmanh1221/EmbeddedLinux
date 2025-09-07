/*
    Lesson2 
    Vu Manh Duong
    FIle in Linux system
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include<stdbool.h>

#define NUMBER_ARG (2)


int main(int argc, char *argv[]){

    
    // Check argument is passed
    if(argc != NUMBER_ARG){
        // Log error
     
        fprintf(stderr, "Error. Don't enough varriable\n");
        exit(EXIT_FAILURE);
    }


    char file_path[1000]; //save directory
    struct stat file_s;   //save status of file


    strncpy(file_path, argv[1], sizeof(file_path) - 1);
    file_path[sizeof(file_path) - 1] = '\0';


    // Get information of file
    if (lstat(file_path, &file_s) != 0) {
        fprintf(stderr, "lstat error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    printf("File Path: %s\n", file_path);

    /*
        Variabe save information
    */

    char * type = "NULL";
    time_t date = 0u;
    unsigned long long fsize = 0;
    char timebuf[100];

    if (S_ISREG(file_s.st_mode)) {
    type = "Regular File";
    } else if (S_ISDIR(file_s.st_mode)) {
        type = "Directory";
    } else if (S_ISLNK(file_s.st_mode)) {
        type = "Symbolic Link";
    } else {
        type = "Other";
    }

    fsize = (unsigned long long)file_s.st_size;
    date = file_s.st_mtime;
    struct tm *time_info = localtime(&file_s.st_mtime);
    if (time_info == NULL) {
        strcpy(timebuf, "Error");
    }
    else if((strftime(timebuf, sizeof(timebuf), "%d-%m-%Y %H:%M:%S", time_info) == 0)){
        strcpy(timebuf, "Error");
    }


    // Print information of file to screen
    printf("%-25s %15s %45s\n","File type","size","Last modified");
    printf("%-25s %15llu %45s\n", type, fsize, timebuf);
    
    return EXIT_SUCCESS;
}