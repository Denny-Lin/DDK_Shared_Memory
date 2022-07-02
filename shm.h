#ifndef __SHM_H__
#define __SHM_H__

int parser(/*int argc,*/ char* argv[]);

//./out -m
//Show memory usage after memset().
int memory_used();

//./out -1
//anonymous mapping
int example();


//"./out -2 -w hello" and then use another shell to "./out -2 -r read"
//"./out -2 -w 1" and then use another shell to "./out -2 -w 2"
//We open a file and map this file in the virtual memory of ./out, and then request a 4k page cache in physical memory. 
//So, we can read the contents written by another bin, since they used the same physical memory in the user space.
//Finally, the opened file will be closed, but deleted by the later ./out until it finishes.
//file mapping
int example2(char rw, char* contents);

//Parent calls child to unzip files and child sends the progress.
int unzip();

#endif /*__SHM_H__*/

