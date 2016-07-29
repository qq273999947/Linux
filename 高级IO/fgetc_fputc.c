#include<stdio.h>
#include<unistd.h>

int main()
{
    char buf[1024];

    while(fgets(buf,1024,stdin) != NULL){
        if(fputs(buf,stdout) == EOF){
            printf("output error\n");
        }
    }
    if(ferror(stdin)){
        printf("input error\n");
    }
    exit(0);
}
