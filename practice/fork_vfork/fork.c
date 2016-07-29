#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>

int main()
{
    pid_t pid = getpid();

    pid_t id = fork();
    if(id == -1){
        printf("fork error! return code is:%d\n",errno);
        exit(2);
    }
    else if(id == 0){
        printf("child pid is: %d\n",getpid());
    }
    else{//father
    
         printf("father pid id: %d\n,return pid is :%d\n",getpid(),id);
    }
    return 0;
}
