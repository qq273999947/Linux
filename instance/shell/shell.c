#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>

int main()
{
    while(1)
    {
      //printf("[enen@shiwo shell]#");
      printf("[%s@localhost shell$", getlogin());
      fflush(stdout);

      char buff[1024];
      memset(buff,'\0',sizeof(buff));

      ssize_t _sz = read(0,buff,sizeof(buff)-1);
      if(_sz > 0){
          buff[_sz - 1] = '\0';
      }
      //printf("%s",buff);

        char *my_argv[64];
        char*p = buff;
        int index = 1;
        my_argv[0] = buff;
        while(*p != '\0'){
            if(*p == ' '){
                *p = '\0';
                ++p;
                my_argv[index++] = p;
            }else{
                ++p;
            }
        }
         my_argv[index] = NULL;
         pid_t id = fork();
         if(id == 0){
          //child
          //printf("I am child:%d\n",getpid());
          //sleep(2);
        //  char *my_argv[] = 
        //  {
        //      "ls",
        //      "-l",
        //      "-i",
        //      "-a",
        //      NULL
        //  };
        //  char* my_env[] = {
        //      "PATH=hello/i/am/fairy",NULL
        //  };
          //execl("/bin/ls","ls","-l","-a","-i",NULL);
          //execv("/bin/ls",my_argv);
          //execle("./hello","ls",NULL,my_env);
          //execlp("ls","ls","-l",NULL);
          execvp(my_argv[0],my_argv);
      }else{
          //father
          //printf("I am father:%d\n",getpid);
          pid_t ret = waitpid(id,NULL,0);
          if(ret > 0){
             // printf("wait success!\n");
          }else{
             // printf("wait failes!\n");
          }
      }
    }
    return 0;
}
