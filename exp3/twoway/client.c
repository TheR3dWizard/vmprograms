#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    struct sockaddr_in addr;
    int sock = 0,valread;
    char buffer[BUFFER_SIZE];
    char msg[BUFFER_SIZE];

    if(!(sock = socket(AF_INET,SOCK_STREAM,0))){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr) <= 0){
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if(connect(sock,(struct sockaddr*)&addr,sizeof(addr)) < 0){
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    while(1){
        printf("Enter message: ");
        scanf("%s",msg);
        send(sock,msg,strlen(msg),0);
        if(strcmp(msg,"exit") == 0){
            printf("Exiting...\n");
            break;
        }
        memset(buffer,0,BUFFER_SIZE);
        valread = read(sock,buffer,BUFFER_SIZE);
        if(strcmp(buffer,"exit") == 0){
            printf("Exiting...\n");
            break;
        }
        printf("%s\n",buffer);
    }
}