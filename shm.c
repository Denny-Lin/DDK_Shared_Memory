#include <stdio.h>
#include "shm.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>

static int SUCCESS = 0;
static int ERROR = -1;

int print(char* argv){
    printf("hello %s\n", argv);
    
    return 0;
}

int example(){
    int value = 5;
    int PAGESIZE = 100;
    int* shared_memory;
    pid_t pid = 0;
    
    shared_memory = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    if(shared_memory == MAP_FAILED){      
        return ERROR;
    }
    
    *shared_memory = 20;
    
    pid = fork();
    
    if(pid == 0){
        value = 10;       
        *shared_memory = 200;    
           
        printf("child: value = %d\n", value);
        printf("child: *shared_memory = %i\n", *shared_memory); 
        
        exit(SUCCESS);
    }
    else if(pid == -1){
        return ERROR;
    }
    else{
        wait(NULL);
        
        printf("parent: value = %d\n", value);   
        printf("parent: *shared_memory = %i\n", *shared_memory); 
    }   
    
    return SUCCESS;
}
