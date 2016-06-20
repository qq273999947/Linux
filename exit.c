#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

int g_val = 0;

int main()
{
    pid_t id = fork();
    if(id < 0){
        exit(1);
    }
    else if(id == 0){
        //child
        printf("child is run,child pid is %d\n",getpid());
        sleep(1);
        exit(100);
    }
    else{
        printf("father is run , father pid is: %d\n",getpid());
    }

    int ret;
    if(wait(&ret) < 0){
        printf("wait error , error code is : %d\n",errno);
        return 1;
    }
    printf("wait success,status code is : %d\n",ret);
    return 0;
}

