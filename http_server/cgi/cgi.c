#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void math(char *arg)
{
    char *end = &arg[strlen(arg) - 1];
    char *arr[64];
    int i = 0;
    while(end > arg){
        if(*end == '='){
            arr[i++] = end+1;
        }
        if(*end == '&'){
            *end = '\0';
        }
        end--;
    }
    arr[i] = NULL;

    printf("<html>");
    printf("<head>");
    printf("</head>");
    printf("<body>");
    int sum = 0;
    int data = 0;
    while(arr[i] != NULL){
        data = atoi(arr[i++]);
        sum += data;
    }

    printf("</body>");
    printf("</html>");
}
int main()
{
    char method[1024];
    char arg[1024];

    memset(method,'\0',sizeof(method));
    memset(atg,'\0',sizeof(arg));

    int content_len = -1;
    if(getenv("REQUEST_METHOD")){
        strcpy(method,getenv("REQUEST_METHOD"));
    }

    if(strcasecmp(methon,"GET") == 0){
        if(getenv("QUERY_STRING")){
            strcpy(arg,getenv(QUERY_STRING));
        }
    }

    else if(strcasecmp(method,"POST") == 0){
        if(getenv("CONTENT_LENGTH")){
            content_len = atoi(getenv("CONTENT_LENGTH"));
            int i = 0;
            for(;i < content_len;++i){
                read(0,&arg[i],1);
            }
            arg[i] = '\0';
        }
        else{
            
                }
        math(arg);
    }
    return 0;
}
