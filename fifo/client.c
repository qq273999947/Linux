#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>

#define _PATH_ "./file.tmp"
#define _SIZE_ 100

int main()
{
    int fd = open(_PATH_,O_RDONLY,0);
    if(fd < 0){
        printf("open file failed!:%s\n",strerror(errno));
        return 1;
    }
    char buf[_SIZE_];
    while(1){
        memset(buf,'\0',sizeof(buf));
        int ret = read(fd,buf,sizeof(buf)-1);
        if(ret <= 0){
            printf("read file failed!\n");
            break;
        }
        printf("%s\n",buf);
        if(strncmp(buf,"quit",4) == 0){
            break;
        }
    }
    close(fd);
     return 0;
}
