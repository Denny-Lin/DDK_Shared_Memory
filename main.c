#include "shm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    //char* test = (char*)calloc(100000000, sizeof(char));//100MB
    
    //char* test = (char*)malloc(100000000 * sizeof(char));//100MB
    //memset(test, 0, 100000000);
    
    //print(argv[1]);
    
    example();
    
    example2_write();
    
    //example2_read();
    
    return 0;
}
