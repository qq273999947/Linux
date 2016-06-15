#include<stdio.h>
#include<string.h>
#include<errno.h>
#define _FILE_NAME_ "./cute"

int main()
{
    FILE* fp = fopen(_FILE_NAME_,"w");
    if(fp == NULL)
    {
        printf("open file %s error,error code is:%d\n",_FILE_NAME_,errno);
        return 1;
    }
    char*str = "Im cute\n";
    int i = 0;
    while(i < 100)
    {
        fwrite(str,1,strlen(str),fp);
        i++;
    }
    fclose(fp);
    return 0;
}
