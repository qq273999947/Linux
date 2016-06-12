#include<stdio.h>
int main()
{
    extern char **environ;
    int i = 0;
    for(;environ[i] != NULL;++i)
    {
        printf("%s\n",environ[i]);
    }
    return 0;
}
