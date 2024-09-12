/* xsh_echo.c - xsh_echo */

#include <xinu.h>
 

shellcmd xsh_touch(int nargs, char *args[])
{
         #if 0
	FILE* fd;
    char *tmp=full_path((char*)args[1]);
    if (tmp==NULL)return -1;
    if ((fd = fopen(tmp,"r"))){
        printf("file found %s\n",tmp);
        fclose(fd);
        return -1;
    }
    fd = fopen(tmp,"w");
    fclose(fd);
    #endif
	return 0;
}
