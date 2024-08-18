#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    int serv_fd, new_sock, valread;
    struct sockaddr_in addr;
    int opt = 1;
    socklen_t addrlen = sizeof(addr);
    char buffer[BUFFER_SIZE] = {0};
    char msg[BUFFER_SIZE];
    if(!(serv_fd = socket(AF_INET,SOCK_STREAM,0))){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    if(setsockopt(serv_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if(bind(serv_fd,(struct sockaddr *)&addr,addrlen) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if(listen(serv_fd,3) <0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){
        printf("Waiting for incoming message...\n\n");  
        if((new_sock = accept(serv_fd,(struct sockaddr *)&addr,&addrlen)) < 0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        while(1){
            memset(buffer,0,BUFFER_SIZE);
            valread = read(new_sock,buffer,BUFFER_SIZE);
            if(strcmp(buffer,"exit") == 0){
                printf("Exiting...\n");
                break;
            }
            printf("%s\n",buffer);
            printf("Enter message: ");
            scanf("%s",msg);
            send(new_sock,msg,strlen(msg),0);
            if(strcmp(msg,"exit") == 0){
            printf("Exiting...\n");
            break;
        }
        }
        printf("Client disconnected\n");
        close(new_sock);
    }
}