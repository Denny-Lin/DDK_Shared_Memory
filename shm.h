#ifndef __SHM_H__
#define __SHM_H__

//test
int print(char* argv[]);


int memory_test();

//anonymous mapping
int example();

int parser(char* argv[]);

//file mapping (write)
int example2_write();

//file mapping (read)
int example2_read();

//Parent calls child to unzip files.
int unzip();

#endif /*__SHM_H__*/

