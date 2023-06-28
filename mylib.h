#ifndef MYLIB_H
#define MYLIB_H
#include <unistd.h>
int fputs(const char *str, FILE *stream);
void* malloc(size_t size);
void free(void *p);
typedef pid_t (*fork_func_t)(void);
struct dirent *(*old_readdir)(DIR *dir);
int close(int fd);
#endif
