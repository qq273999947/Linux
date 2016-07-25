#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include <arpa/inet.h>

int main()
{
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0){
        printf("creat socket error\n");
        return 1;
    }

    struct sockaddr_in sever_socket,client_socket;
    bzero(&sever_socket,sizeof(sever_socket));
    sever_socket.sin_family = AF_INET;
    sever_socket.sin_addr.s_addr = htonl(INADDR_ANY);
    sever_socket.sin_port = htons(8080);

    if(bind(sock,(struct sockaddr*)&sever_socket,sizeof(struct sockaddr_in)) < 0){
        perror("bind error\n");
        close(sock);
        return 2;
    }
    
    if(listen(sock,10) < 0){
        perror("listen error\n");
        close(sock);
        return 3;
    }

    printf("bind and listen success!wait accept...\n");

    while(1){
        socklen_t len =0;
        int client_sock = accept(sock,(struct sockaddr*)&client_sock,&len);
        if(client_sock < 0){
            perror("accept error\n");
            close(sock);
            return 4;
        }
        
        char buf_ip[INET_ADDRSTRLEN];
        memset(buf_ip,'\0',sizeof(buf_ip));
        inet_ntop(AF_INET,&client_socket.sin_addr,buf_ip,sizeof(buf_ip));

        printf("get connect,ip is : %s port is : %d\n",buf_ip,ntohs(client_socket.sin_port));

        while(1){
            char buf[1024];
            memset(buf,'\0',sizeof(buf));

            read(client_sock,buf,sizeof(buf));
            printf("client say : %s\n",buf);

            printf("sever say :\n");
            memset(buf,'\0',sizeof(buf));
            fgets(buf,sizeof(buf),stdin);
            buf[strlen(buf) - 1] = '\0';
            write(client_sock, buf, strlen(buf)+1);
            printf("please wait...\n");
        }
    }
    close(sock);
    return 0;
}
