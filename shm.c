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

int print(char* argv[]){
    printf("hello %s\n", argv[1]);
    
    return 0;
}

int memory_test(){
    char* test1 = (char*)calloc(100000000, sizeof(char));//100MB
    
    getchar();
    
    char* test2 = (char*)malloc(100000000 * sizeof(char));//100MB
    memset(test2, 0, 100000000);
    
    return SUCCESS;
}

int parser(char* argv[]){
    if(argv[1]==NULL) return ERROR;
      
    switch(argv[1][0]){
        case 't':
            print(argv);
            break;
        case 'e':
            example();
            break;    
        case 'w':
            example2_write();
            break;
        case 'r':
            example2_read();
            break;    
        default: 
            printf("Do not have %c\n", argv[1][0]);
            break;    
    }
    
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

int example2_write(){
    int fd;
    int PAGESIZE = 4096;
    char* shared_memory;   
    static char* buf= "Hello World!";
    
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
    int count = 10;
    do{      
        memcpy(shared_memory, buf+count, strlen(buf));
        printf("%s\n", shared_memory);
        sleep(1);
    }while(count--);
     
    unlink("example2");   
    
    munmap(shared_memory, PAGESIZE);
           
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
    int count = 20;
    do{          
        printf("%s\n", shared_memory);
        sleep(1);
    }while(count--);
    
    unlink("example2");
    
    munmap(shared_memory, PAGESIZE);

    return SUCCESS;
}



