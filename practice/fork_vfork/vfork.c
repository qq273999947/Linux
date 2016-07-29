#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

int g_val = 0;

void func()
{
    printf("child exit\n");
}

int main()
{
    int _val = 0;
    pid_t id = vfork();
    if(id < 0)
    {
        exit(1);
    }
    else if(id == 0)
    {
        //child
        atexit(func);
        printf("I am child..\n");
        ++g_val;
        ++_val;
        sleep(3);
        exit(0);
    }
    else{
        printf("I am father..\n");
        printf("father exit,g_val = %d,_val = %d\n",g_val,_val);
    }
    return 0;
}
