#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>

void creat_daemon()
{
    int i,fd;
    pid_t id;
    struct sigaction sa;
    umask(0);//第一步：调用umask将文件模式创建屏蔽字设置为0.
    if((id = fork()) < 0){
        printf("fork error!\n");
        return;
    }else if(id != 0){
        exit(0);//第二步：调用fork，父进程退出。保证子进程不是话首进程，从而保证后续不和其他终端关联。
    }

    setsid();//第三步：设置新会话。 

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGCHLD,&sa,NULL) < 0){
        //注册子进程退出忽略信号
        return;
    }

    if((id = fork()) < 0){
        printf("fork 2 error!\n");
        return;
    }else if(id != 0){
         exit(0);
    }

    if(chdir("/") < 0){
        //第四步：更改工作目录到根目录 
        printf("child dir error\n");
        return;
    }
    close(0);
    fd = open("/dev/null",O_RDWR);//关闭标准输入，重定向所有标准（输入输出错误）到/dev/NULL
    dup2(fd,1);
    dup2(fd,2);
}

int main()
{
    creat_daemon();
    while(1){
        sleep(1);
    }
}
