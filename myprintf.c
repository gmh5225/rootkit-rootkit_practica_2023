#include<unistd.h>
int puts(const char* str);
void printf(const char* str)
{
	puts(str);
	sleep(10);
	puts("WTF");
	return;
}
