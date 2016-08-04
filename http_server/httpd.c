#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<pthread.h>
#include<string.h>
#include<sys/sendfile.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

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

static void bad_request(int sock)
{
    char buf[1024];
    sprintf(buf,"HTTP/1.0 400 BAD REQUEST\r\n");
    send(sock,buf,strlen(buf),0);
    sprintf(buf,"Content-type: text/html\r\n");
    send(sock,buf,strlen(buf),0);
    sprintf(buf,"\r\n");
    send(sock,buf,strlen(buf),0);  
    sprintf(buf,"<html><br><p>your enter message is a bad request</p></br></html>\r\n");
    sprintf(sock,buf,strlen(buf),0);
}

static void not_found(int sock)
{
    char buf[1024];
    sprintf(buf,"HTTP/1.0 404 NOT FOUND\r\n");
    send(sock,buf,strlen(buf),0);
    sprintf(buf,"Content-type: text/html\r\n");
    send(sock,buf,strlen(buf),0);
    sprintf(buf,"\r\n");
    send(sock,buf,strlen(buf),0);  
    sprintf(buf,"<html><br><p>your enter message is a not found</p></br></html>\r\n");
    sprintf(sock,buf,strlen(buf),0);
}
static void echo_errno(int sock,int num)
{
    switch(num){
        case 404:
            not_found(sock);
            break;
        case 400:
            bad_request(sock);
            break;
    }
    close(sock);
}
static int echo_www(int sock,const char* path,ssize_t size)
{
    int fd = open(path,O_RDONLY);
    if(fd < 0){
        echo_errno(sock);
        return -1;
    }
    char buf[_SIZE_];
    memset(buf,'\0',sizeof(buf));
    sprintf(buf,"HTTP/1.0 200 OK\r\n\r\n");
    send(sock,buf,strlen(buf),0);

    if(sendfile(sock,fd,NULL,size) < 0){
        echo_errno(sock);
        return -2;
    }
    close(fd);
}
static void clear_head(int sock)
{ 
    int ret = 0;
    char buf[_SIZE_];
    do{
        ret =  get_line(sock,buf,sizeof(buf));
    }while(ret > 0 && strcmp(buf,"\n"));
}

static int execut_cgi(int sock,const char *path,const char *method,const char* query_string)
{
    int conten_len = -1;//正文长度

    char buf[_SIZE_];

    if(strcasecmp(method,"GET") == 0){
        clear_head(sock);
    }else{//POST
          int ret = 0;
          memset(buf,'\0',sizeof(buf));
          do{
              ret = get_line(sock,buf,sizeof(buf));
              if(ret > 0 && strncasecmp(buf,"Content-Lenghth: ",16) == 0){
                  conten_len = atoi(&buf[16]);
              }
          }while((ret > 0) && strcmp(buf,"\n") != 0);
          if(conten_len == -1){
              echo_errno(sock);
              return -2;
          }
    }

    //cgi
    printf("Content_Length:%d\n",content_len);
    sprintf(buf,"HEEP/1.0 200 OK\r\n\r\n");
    send(sock,buf,strlen(buf),0);

    int cgi_input[2];
    int cgi_output[2];

    //创建子进程执行cgi脚本
    if(pipe(cgi_input) < 0){
        echo_errno(sock);
        return -3;
    }

    if(pipe(cgi_output) < 0){
        echo_errno(sock);
        return -4; 
    }

    char query_env[_SIZE_];
    char method_env[_SIZE_];
    char content_len_env[_SIZE_];
    memset(method_env, '\0', sizeof(method_env));
    memset(query_env, '\0', sizeof(query_env));
    memset(content_len_env, '\0', sizeof(content_len_env));

    pid_t id = fork();
    if(id == 0){//child输入
          close(cgi_input[1]);
          close(cgi_output[0]);

          dup2(cgi_input[0],0);//先关0，再复制cgi的0到0
          dup2(cgi_output[1],1);
          sprintf(method_env,"REQUEST_METHOD=%s",method);
          putenv(method_env);

         if(strcasecmp(method,"GET") == 0){
            sprintf(query_env,"QUERY_STRING=%d",query_string);
            putenv(query_env);
         }else{
             //POST
             sprintf(content_len_env,"CONTENT_LENTGHT=%d",conten_len);
             putenv(content_len_env);
         }
          execl(path,path,NULL);
          exit(1);
    }else{//father输出
          close(cgi_input[0]);
          close(cgi_output[1]);

          int i = 0;
          char c = '\0';
          if( strcasecmp(method,"POST") == 0 ){
              for(;i < conten_len;++i){
                  recv(sock,&c,1,0);
                  write(cgi_input[1],&c,i);
              }
          }

          while( read(cgi_output[0],&c,1) > 0 ){
              send(sock,&c,1,0);
          }

          waitpid(id,NULL,0);

          close(cgi_input[1]);
          close(cgi_output[0]);
    }
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
    //close(sock);
    return NULL;
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
    if(strcasecmp(method,"GET") != 0 && strcasecmp(method,"POST") != 0){
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
        ++i,++j;
    }
    printf("method : %s , url_path : %s\n",method,url);


    char*start = url;
    char* query_string = 0;
    int cgi = 0;

    if(strcasecmp(method,"POST") == 0){
        cgi = 1;
    }
    if(strcasecmp(method,"GET") == 0){
        query_string = url;
        while(*query_string != '\0' && query_string != '?'){
            query_string++;
        }
    }

    if(*query_string == '?'){
        cgi = 1;
        *query_string = '\0';
        query_string++;
    }

    sprintf(path,"htdoc/%s",url);
    if(path[strlen(path) - 1] == '/');
    {
        strcat(path,"index.html");
    }

   struct stat st;
   if(stat(path,&st) < 0){//defalut->htdoc/index.html
       echo_errno(sock);
       return (void*)-3;       
    }else{
           if(S_ISDIR(st.st_mode) ){
             strcpy(path,"htdoc/index.html");
           }else if( (st.st_mode & S_IXUSR ) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH)){
                   cgi = 1;
           }else{
           }
    }

    if(cgi){ //cgi
        execut_cgi(sock,path,method,query_string);
    }else{
        clear_head(sock);
        echo_www(sock,path,st.st_size);
    }
    //printf("path: %s\n",path);
    
    close(sock);//无连接，处理完立即关掉
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
