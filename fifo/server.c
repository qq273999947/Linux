#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>

#define _PATH_ "./file.tmp"
#define _SIZE_ 100

int main()
{
    int ret = mkfifo(_PATH_,0666|S_IFIFO);
    if(ret == -1){
        printf("mkfifo failed!\n");
        return 1;
    }
    int fd = open(_PATH_,O_WRONLY|O_CREAT,0644);
    if(fd < 0){
        printf("open error:%s\n",strerror(errno));
    }
    char buf[_SIZE_];
    while(1){
        printf("client Enter:");
        fflush(stdout);
        memset(buf,'\0',sizeof(buf));
        scanf("%s",buf);
        int len = strlen(buf);
        buf[len] = '\0';
        int ret = write(fd,buf,strlen(buf));
        if(ret < 0){
            printf("write error!:%s\n",strerror(errno));
            break;
        }
        if(strncmp(buf,"quit",4) == 0){
            break;
        }
    }
    close(fd);
    return 0;
}
