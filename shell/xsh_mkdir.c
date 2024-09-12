/* xsh_echo.c - xsh_echo */

#include <xinu.h>
 

shellcmd xsh_mkdir(int nargs, char *args[])
{
         #if 0
	FL_DIR dirstat;
    char *tmp=full_path((char*)args[1]);
    if (tmp==NULL)return -1;
    if(fl_opendir(tmp,&dirstat)){
        fl_closedir(&dirstat);
        printf("dir found %s\n",tmp);
        return 0;
    }
    
    mkdir(tmp);
    #endif
	return 0;
}
