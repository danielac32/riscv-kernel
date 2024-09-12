

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include "littlefs.h"
#include "melf.h"
littlefs_t disk;


int replacestr(char *line, const char *search, const char *replace)
{
   int count;
   char *sp; // start of pattern

   //printf("replacestr(%s, %s, %s)\n", line, search, replace);
   if ((sp = strstr(line, search)) == NULL) {
      return(0);
   }
   count = 1;
   int sLen = strlen(search);
   int rLen = strlen(replace);
   if (sLen > rLen) {
      // move from right to left
      char *src = sp + sLen;
      char *dst = sp + rLen;
      while((*dst = *src) != '\0') { dst++; src++; }
   } else if (sLen < rLen) {
      // move from left to right
      int tLen = strlen(sp) - sLen;
      char *stop = sp + rLen;
      char *src = sp + sLen + tLen;
      char *dst = sp + rLen + tLen;
      while(dst >= stop) { *dst = *src; dst--; src--; }
   }
   memcpy(sp, replace, rLen);

   count += replacestr(sp + rLen, search, replace);

   return(count);
}


void ls(char *s){
 

    struct lfs_info info;
   // char *tmp=malloc(200);

    lfs_dir_t *dir = disk.openDir(s);
    if(dir==0){
        printf("not a directory %s",s);
    }
    while (disk.readDir(dir, &info)> 0){
        if (strcmp(info.name, ".") && strcmp(info.name, "..") && strcmp(info.name, "//")){
 				if (info.type == LFS_TYPE_DIR){
	               	printf(" /%s\n",info.name);
	            }else{
	                printf(" %s %d \n",info.name,info.size);
	            }
        }
    }
    disk.closeDir(dir);
}




#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"



void create_file(char *tmp,char *name){
char tmp2[100];
char tmp3[100];
lfs_file_t *fs;
int ret;

strcpy(tmp2,tmp);
strcat(tmp2,name);

strcpy(tmp3,"mkfs/kernel/");
strcat(tmp3,name);
FILE* fpSrc = fopen(tmp3, "r");
if(fpSrc==NULL){
    printf("error open file %s\n",tmp3 );
}
unsigned long lsize = 0;
char *p;
fseek(fpSrc, 0, SEEK_END);
lsize = ftell(fpSrc);
rewind(fpSrc);
p=(char*)malloc(lsize);
fread(p, sizeof(char), lsize, fpSrc);

fclose(fpSrc);

fs=disk.open(tmp2,LFS_O_CREAT);
if((ret=disk.write(p,lsize,fs))<0){
   printf("error (%d)\n",ret );
}
disk.close(fs);
free(p);
}

void create_file2(char *tmp,char *name){
char tmp2[100];
char tmp3[100];
lfs_file_t *fs;
int ret;
strcpy(tmp2,tmp);
strcat(tmp2,"/");
strcat(tmp2,name);

strcpy(tmp3,"mkfs/kernel/");
strcat(tmp3,tmp2);
FILE* fpSrc = fopen(tmp3, "r");
if(fpSrc==NULL){
    printf("error open file %s\n",tmp3 );
}
unsigned long lsize = 0;
char *p;
fseek(fpSrc, 0, SEEK_END);
lsize = ftell(fpSrc);
rewind(fpSrc);
p=(char*)malloc(lsize);
fread(p, sizeof(char), lsize, fpSrc);
fclose(fpSrc);

//printf("file->%s\n",tmp2 );
//replacestr(tmp2,".elf",".bin");
//printf("file->%s %d\n",tmp2, ximg.size);
fs=disk.open(tmp2,LFS_O_CREAT);
if((ret=disk.write(p,lsize,fs))<0){
   printf("error (%d)\n",ret );
}
disk.close(fs);
free(p);
 

}




void show_dir_content(char * path)
{
  DIR * d = opendir(path); // open the path
  if(d==NULL) return; // if was not able, return
  struct dirent * dir; // for the directory entries
  while ((dir = readdir(d)) != NULL) // if we were able to read somehting from the directory
    {
      if(dir-> d_type != DT_DIR){ // if the type is not directory just print it with blue color
         char tmp[100];
        // char tmp2[100];
        // lfs_file_t *fs;
         strcpy(tmp,path);
         replacestr(tmp,"mkfs/kernel/","/");
         replacestr(tmp,"mkfs/kernel","");
         replacestr(tmp,"/","");
         if(!strcmp(tmp,"")){
            printf("%s%s%s\n",BLUE, tmp,dir->d_name);
            create_file(tmp,dir->d_name);
            //strcpy(tmp2,tmp);
            //strcat(tmp2,dir->d_name);
            //fs=disk.open(tmp2,LFS_O_CREAT);
            //disk.close(fs);
         }else{
            printf("%s%s/%s\n",BLUE, tmp,dir->d_name);
            create_file2(tmp,dir->d_name);
            //strcpy(tmp2,tmp);
            //strcat(tmp2,"/");
            //strcat(tmp2,dir->d_name);
            //fs=disk.open(tmp2,LFS_O_CREAT);
            //disk.close(fs);
         }
      }else
      if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 ) // if it is a directory
      {
        printf("%s%s\n",GREEN, dir->d_name); // print its name in green
        disk.mkdir(dir->d_name);
        char d_path[255]; // here I am using sprintf which is safer than strcat
        sprintf(d_path, "%s/%s", path, dir->d_name);
        show_dir_content(d_path); // recall with the new path
      }
    }
    closedir(d); // finally close the directory
}




/*void show_dir_content2(char * path)
{
  DIR * d = opendir(path); // open the path
  if(d==NULL) return; // if was not able, return
  struct dirent * dir; // for the directory entries
  while ((dir = readdir(d)) != NULL) // if we were able to read somehting from the directory
    {
      if(dir-> d_type != DT_DIR){ // if the type is not directory just print it with blue color
 
         printf("%s%s\n",BLUE,dir->d_name);

           
      }else
      if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 ) // if it is a directory
      {
        printf("%s%s\n",GREEN, dir->d_name); // print its name in green
        char d_path[255]; // here I am using sprintf which is safer than strcat
        sprintf(d_path, "%s/%s", path, dir->d_name);
        show_dir_content(d_path); // recall with the new path
      }
    }
    closedir(d); // finally close the directory
}*/

int main(int argc, char const *argv[])
{
	
    #if 1
    int ret;
    int disk_free;
    littlefs_init(&disk, &config, 500000);
    #if 1
    printf("startup_test: format    ");
    ret = disk.format(disk.lfs, &config);
    if(ret) 
    {
        printf("[FAIL]\n");
        printf("Error Code: %d\n", ret);
        return -1;
    }
    printf("[ OK ]\n");
    #endif
    printf("startup_test: mount     ");
    ret = disk.mount(disk.lfs, &config);
    if(ret) 
    {
        printf("[FAIL]\n");
        printf("Error Code: %d\n", ret);
        return -1;
    }

    printf("[ OK ]\n");
    disk_free = disk.free();
    printf("startup_test: available size = %d Bytes\n", disk_free);

    
    
    /*if(!strcmp(argv[1],"-binary")){
       printf("convert to bin\n");
       printf("%s\n", NORMAL_COLOR);
       show_dir_content2(argv[2]);
       printf("%s\n", NORMAL_COLOR);
    }else{
     
    }*/
    //printf("%s\n", NORMAL_COLOR);
    //show_dir_content2(argv[1]);
    //printf("%s\n", NORMAL_COLOR);



    printf("%s\n", NORMAL_COLOR);
    show_dir_content(argv[1]);
    printf("%s\n", NORMAL_COLOR);

    /***********************************************************/




       ls("/");

       //free(p);
       disk.downloadfs(&config);
#endif



	return 0;
}

