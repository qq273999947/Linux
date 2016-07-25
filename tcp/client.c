#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"

int main(int argc,char *argv[])
{
    if( argc != 2 ){
        printf("please use : client IP\n");
        return 1;
    }
    char *str = argv[1];
    char buf[1024];
    memset(buf,'\0',sizeof(buf));
    
    struct sockaddr_in server_sock;
    int sock = socket(AF_INET,SOCK_STREAM,0);
    bzero(&server_sock,sizeof(server_sock));
    server_sock.sin_family = AF_INET;

    inet_pton(AF_INET,SERVER_IP,&server_sock.sin_addr);
    server_sock.sin_port = htons(SERVER_PORT);

    int ret = connect(sock,(struct sockaddr*)&server_sock,sizeof(server_sock));
    if(ret < 0){
        printf("connect failed:%s\n",strerror(errno));
        return 1;
    }
    
    printf("connect success!\n");

    while(1){
        printf("client say:");
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1] = '\0';

        write(sock,buf,sizeof(buf));
        if(strncasecmp(buf,"quit",4) == 0){
            printf("qiut\n");
            break;
        }
        printf("please wait...\n");
        read(sock,buf,sizeof(buf));
        printf("server say : %s\n",buf);
    }
    close(sock);
    return 0;
}
