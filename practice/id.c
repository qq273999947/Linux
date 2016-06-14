#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
int main()
{
    uid_t uid;//real user
    uid_t euid;//effective user
    pid_t ppid;
    pid_t pid = fork();
    if(pid < 0)
    {
        printf("%d\n",errno);
        exit(2);
    }
    else if(pid == 0)
    {
     //child
        uid = getuid();
        euid = geteuid();
        printf("child->pid:%d,ppid:%d,uid:%d,euid:%d\n",getpid(),getppid(),uid,euid);
        exit(1);
    }
    else{
        //father
        uid = getuid();
        euid = geteuid();
        printf("father->pid:%d,ppid:%d,uid:%d,euid:%d\n",getpid(),getppid(),uid,euid);
        sleep(2);
    }
    return 0;
}
