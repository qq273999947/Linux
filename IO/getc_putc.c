#include<stdio.h>
#include<unistd.h>

int main()
{
    int c;
    while(c = getc(stdin) != EOF){
        if(putc(c,stdout) == EOF){
            printf("output error\n");
        }
    }
    if(ferror(stdin)){
        printf("input error\n");
    }
    exit(0);
}
