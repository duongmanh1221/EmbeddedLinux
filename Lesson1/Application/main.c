#include "strutils.h"



int main (){
    char * test1 = "abcd" ;
    char * test2 = "  aad  ";
    char * test3 = "834" ;

    char * result1 = str_reverse(test1);
    char * result2 = str_trim(test2);
    int result3 = str_to_int(test3);

    printf("\nFuntion 1 : %s",result1);
    printf("\nFuntion 2 : %s",result2);
    printf("\nFuntion 3 : %d",result3);

    free(result1);
    free(result2);

    return 0;

}