#include<stdio.h>
#include<stdlib.h>

int main()
{
    int id = fork();
    if(id > 0){
        printf("Parent is sleeping..");
        sleep(30);
    }

    if(id  == 0){
        printf("Child process is done ..");

        exit(EXIT_SUCCESS);
    }
}
