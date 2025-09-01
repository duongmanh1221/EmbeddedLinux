#include "strutils.h"

static bool isValidChar(char symbol);

char *str_reverse(char const *str){
    int len = strlen(str);
    int i;
    char * buff = (char *)malloc(sizeof(char)*(len+1));
    for(i=0;i<len;i++){
        buff[i] = str[len-i-1];
    }
    buff[len] = '\0';
    return buff;
}
char *str_trim(const char *str) {
    int len = strlen(str);
    int start = 0, end = len - 1;
    int count = 0;
    int i;

  
    while (start < len && str[start] == ' ') {
        start++;
    }

  
    while (end > start && str[end] == ' ') {
        end--;
    }

    
    int new_len = end - start + 1;

   
    char *buff = (char *)malloc(sizeof(char) * (new_len + 1));
    if (!buff) return NULL;

    
    for (i = start; i <= end; i++) {
        buff[count++] = str[i];
    }
    buff[count] = '\0';

    return buff;
}

int str_to_int(char const *str){
    int sum = 0;
    int len = strlen(str);
    int i;

    for(i=0;i<len;i++){
        if(isValidChar(str[i])==false){
            sum = 0;
            break;
        }
        else {
            sum = sum + (str[i] - '0');
        }
    }

    return sum;
}

static bool isValidChar(char symbol){
    bool result;

    if(symbol>='0'&&symbol<='9'){
        result = true ;
    }
    else{
        result = false;
    }

    return result;
    
}
