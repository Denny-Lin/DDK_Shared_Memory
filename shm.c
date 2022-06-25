#include <stdio.h>
#include "shm.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int PAGESIZE = 100;

int print(char* argv){
    printf("hello %s\n", argv);
    
    return 0;
}

int example(){
    int value = 5;
    int* shared_memory = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    *shared_memory = 20;
    
    if(fork()==0){
        value = 10;       
        *shared_memory = 200;    
           
        printf("child: value = %d\n", value);
        printf("child: *shared_memory = %i\n", *shared_memory); 
    }
    else{
        wait(NULL);
        
        printf("parent: value = %d\n", value);   
        printf("parent: *shared_memory = %i\n", *shared_memory); 
    }   
    
    return 0;
}
