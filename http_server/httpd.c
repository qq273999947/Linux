#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<pthread.h>
#include<string.h>

#define _SIZE_ 1024

static void Usage(const char* proc)
{
    printf("Usage: %s [ip] [port]\n",proc);
}

static int startup(char* ip ,int port)
{
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0){
        perror("sock");
    }

    int opt = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = inet_addr(ip);

    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
        perror("bindd");
        exit(2);
    }

    if(listen(sock,5) < 0){
        perror("listen");
        exit(3);
    }

    return sock;
}

static int get_line(int sock,char buf[],int len)
{
    if(!buf || len < 0)
    {
        return -1;
    }
    
    int i = 0;
    char ch;
    while(i < len && ch != '\n') {
        recv(sock,&ch,1,0);
        if(ch == '\r'){
             recv(sock,&ch,1,MSG_PEEK);
             if(ch == '\n'){
                 recv(sock,&ch,1,0);
             }else{
                 ch = '\n';
             }
        }
        buf[i++] = ch;
    }
    buf[i] = '\0';
    return i;
}

static void* accept_request(void *arg)
{
    int sock = (int)arg;
    int ret = 0;
    char buf[_SIZE_];
#ifdef _DEBUG_
    do{
        get_line(sock,buf,_SIZE_);
        printf("%s",buf);
    }while(ret > 0 && strcmp(buf,"\n"));
#endif

    char method[_SIZE_/10];
    char path[_SIZE_];
    char url[_SIZE_];

    memset(method,'\0',sizeof(method));
    memset(url,'\0',sizeof(url));
    memset(path,'\0',sizeof(path));
    memset(buf,'\0',sizeof(buf));

    ret = get_line(sock,buf,sizeof(buf));
    if(ret < 0)
    {
        //echo_errno(sock);
        return (void*)-1;
    }

    //1.get method
    int i = 0,j = 0;
    while((!isspace(buf[j])) && (i < sizeof(method)-1 ) && (j < sizeof(buf))){
        method[i] = buf[i];
        ++i,++j;
    }

    method[i] = '\0';
    //printf("method : %s\n",method);
    
    //2.check method
    if(strcmp(method,"GET") != 0 && strcmp(method,"POST") != 0){
        //echo_errno(sock);
        return (void*)-2;
    }

    //3.get url
    //先越过空格
    while(isspace(buf[j])){
        ++j;
    }

    i = 0;
    while((!isspace(buf[j])) && (i < sizeof(url) - 1) && (j < sizeof(buf))){
        url[i] = buf[j];
        ++i;
        ++j;
    }
    printf("method : %s , url_path : %s\n",method,url);

    char*start = url;
    char* query_string = 0;
    int cgi = 0;
    while(*start != '\0')
    {
        if(*start == '?'){
            cgi = 1;
            *start = '\0';
            query_string = start+1;
            break;
        }
        start++;
    }

    sprintf(path,"htdoc/%s",url);
    if(path[strlen(path) - 1] == '/');
    {
        strcat(path,"index.html");
    }
    printf("path: %s\n",path);
    return (void*)0;
}

int main(int argc,char *argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    int listen_sock = startup(argv[1],atoi(argv[2]));

    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    while(1){
        int new_fd = accept(listen_sock,(struct sockaddr*)&peer,&len);
        if(new_fd < 0){
            perror("accept");
            continue;
        }
        pthread_t id;
        pthread_create(&id,NULL,accept_request,(void*)new_fd);

        pthread_detach(id);
    }
    return 0;
}
