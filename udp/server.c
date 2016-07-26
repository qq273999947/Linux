#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

int main(int argc,char *argv[])
{
    if(argc != 3){
        printf("Please Enter: %s [ip] [port]",argv[0]);
        return 1;
    }
    
    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock < 0){
        perror("socket error");
        return 2;
    }

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[2]));
    local.sin_addr.s_addr = inet_addr(argv[1]);


    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
        perror("bind error");
        return 3;
    }

    int done = 0;
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);

    char buf[1024];
    while(!done){
        memset(buf,'\0',sizeof(buf));
        recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr*)&peer,&len);
        printf("#########################\n");
        printf("get a client,socket: %s:%d\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port));

        printf("client : %s ,echo client!\n",buf);
        printf("#########################\n");
        sendto(sock,buf,sizeof(buf),0,(struct sockaddr*)&peer,len);

    }
    return 0;
}
