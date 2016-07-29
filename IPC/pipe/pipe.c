#include<stdio.h>
#include<errno.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>

int main()
{
    int pipe_fd[2] = {-1,-1};
    if(pipe(pipe_fd) < 0){
        printf("pipe error, %s\n",strerror(errno));
        return 1;
    }

    pid_t id = fork();
    if(id == 0){
        //child write
        close(pipe_fd[0]);

        char *msg = "child write:enenshiwo\n";
        while(1){
            write(pipe_fd[1],msg,strlen(msg));
            sleep(1);
        }
    }else{
        //father read
        close(pipe_fd[1]);

        char buf[1024];
        while(1){
            buf[0] = '\0';
            ssize_t _sz = read(pipe_fd[0],buf,sizeof(buf) - 1);

            if(_sz > 0){
                buf[_sz] = '\0';
            }

            printf("father read : %s\n",buf);
        }
        wait(NULL);
    }
    return 0;
}
