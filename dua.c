#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>

pthread_t t;

static const char *dirpath = "/home/camiliamay6/shift4";

void* join(void *arg){
   pthread_t id = pthread_self();
   if(pthread_equal(id,t)){
      DIR *d;
      struct dirent *de;
      d = opendir(dirpath);
      char dname[1000];
      strcpy(dname, dirpath);
      mkdir(strcat(dname,"/Videos"),0777);
      while((de=readdir(d))!=NULL){
         char fname[1000],ch;
         if(strcmp(strrchr(de->d_name,'.'),".001")==0){
            strncpy(fname,de->d_name,strlen(de->d_name)-4);
         }
         else if(strcmp(strstr(de->d_name,fname),fname)==0){
            FILE *fr, *fw;
            fw = fopen(fname,"w");
            fr = fopen(de->d_name, "r");
            while((ch = fgetc(fr))!=EOF) fputc(ch,fw);
            fclose(fr);
            fclose(fw);
            rename(fname,strcat("/Videos/",fname));
         }
      }
     closedir(d);
   }
   sleep(10);
   return NULL;
}

static int xmp_getattr(const char *path, struct stat *stbuff){
   int res;
   char fpath[1000];
   sprintf(fpath,"%s%s",dirpath,path);
   res = lstat(fpath,stbuff);
   if(res==-1) return -errno;
   return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
   char fpath[1000];
   if(strcmp(path,"/")==0){
      path=dirpath;
      sprintf(fpath,"%s",path);
   }
   else sprintf(fpath,"%s%s", dirpath,path);
   DIR *dp;
   struct dirent *de;
   (void) offset;
   (void) fi;
   dp = opendir(fpath);
   if(dp==NULL) return -errno;
   while((de = readdir(dp))!=NULL){
      if(strcmp(de->d_name,"Videos")==0){
         rmdir(de->d_name);
      }
      else{
         struct stat st;
         memset(&st,0,sizeof(st));
         st.st_ino = de->d_ino;
         st.st_mode = de->d_type << 12;
         if(filler(buf,de->d_name,&st,0)) break;
      }
   }
   return 0;
}

static int xmp_rmdir(const char *path){
   int res;
   char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
   res=rmdir(fpath);
   if(res == -1) return -errno;
   return 0;
}
static struct fuse_operations xmp_oper = {
   .readdir = xmp_readdir,
   .getattr = xmp_getattr,
   .rmdir = xmp_rmdir,
};

int main(int argc, char *argv[]){
   pthread_create(&(t),NULL,&join,NULL);
   pthread_join(t,NULL);
   umask(0);
   return fuse_main(argc, argv, &xmp_oper, NULL);
}
