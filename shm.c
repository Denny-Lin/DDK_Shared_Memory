#include <stdio.h>
#include "shm.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define SUCCESS 0
#define ERROR -1

int parser(char* argv[]){
    if(argv[1]==NULL || argv[1][0]!='-') return ERROR;
      
    switch(argv[1][1]){
        case 'm':
            memory_used();
            break;
            
        case '1':
            example();
            break;  
              
        case '2':
            if(argv[2]==NULL || argv[2][0]!='-') return ERROR;
            
            argv[2][1]=='w'?example2('w'):argv[2][1]=='r'?example2('r'):NULL;
            
            break; 
              
        default: 
            printf("Do not have %c\n", argv[1][0]);
            break;    
    }
    
    return SUCCESS;
}

int memory_used(){
    char* test = (char*)malloc(100000000 * sizeof(char));//100MB
    
    system("free -m");
    printf("\n");
    
    memset(test, 0, 100000000);
    
    //used +100
    system("free -m");
    
    return SUCCESS;
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
    
    munmap(shared_memory, PAGESIZE);
    
    return SUCCESS;
}

int example2(char rw){
    int fd;
    int PAGESIZE = 4096;
    char* shared_memory;
    static char* buf= "Hello World!";

    fd = open("example2", (rw=='w')?O_RDWR|O_CREAT:O_RDWR, 0777);

    if(fd<0){
        printf("Can't open the file.\n");
        return ERROR;
    }
    
    ftruncate(fd, PAGESIZE);//       
   
    shared_memory = mmap(NULL, PAGESIZE, (rw=='w')?PROT_WRITE:PROT_READ, MAP_SHARED, fd, 0);
    
    if(shared_memory == MAP_FAILED){      
        printf("mmap() fail.\n");
        return ERROR;
    } 
      
    close(fd);   
     
    int count = 10;
    do{       
        (rw=='w')?memcpy(shared_memory, buf+count, strlen(buf)):NULL; 
              
        printf("%s\n", shared_memory);
        sleep(1);
    }while(count--);
 
    unlink("example2");
    
    munmap(shared_memory, PAGESIZE);

    return SUCCESS;
}


