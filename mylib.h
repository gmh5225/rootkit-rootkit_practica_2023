#ifndef MYLIB_H
#define MYLIB_H
#include <unistd.h>
int fputs(const char *str, FILE *stream);
void* malloc(size_t size);
void free(void *p);
typedef pid_t (*fork_func_t)(void);
struct dirent *(*old_readdir)(DIR *dir);
//FILE *(*orig_fopen)(const char *pathname, const char *mode);
//int open(const char *pathname, int flags);
//ssize_t read(int fd, void *buf, size_t count);
#endif
