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
    
    munmap(shared_memory, PAGESIZE);
    
    return SUCCESS;
}

int example2_write(){
    int fd;
    int PAGESIZE = 4096;
    char* shared_memory;
    char* buf = "Hello World!";
    
    fd = open("example2",O_RDWR | O_CREAT, 0777);
    
    if(fd<0){
        printf("Can't open the file.\n");
        return ERROR;
    }
    
    ftruncate(fd, PAGESIZE);
    shared_memory = mmap(NULL, PAGESIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    
    if(shared_memory == MAP_FAILED){  
        printf("mmap() fail.\n");    
        return ERROR;
    }   
    
    close(fd);
    
    //Write something
    memcpy(shared_memory, buf, strlen(buf));
    printf("%s\n", shared_memory);
      
    getchar();//puase
    
    unlink("example2");
           
    return SUCCESS;
}

int example2_read(){
    int fd;
    int PAGESIZE = 4096;
    char* shared_memory;
    
    fd = open("example2",O_RDONLY);
    
    if(fd<0){
        printf("Can't open the file.\n");
        return ERROR;
    }
    
    shared_memory = mmap(NULL, PAGESIZE, PROT_READ, MAP_SHARED, fd, 0);
    
    if(shared_memory == MAP_FAILED){      
        printf("mmap() fail.\n");
        return ERROR;
    } 
      
    close(fd);   
    
    //Read something 
    printf("%s\n", shared_memory);

    return SUCCESS;
}



