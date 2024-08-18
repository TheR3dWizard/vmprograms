#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void createclient(int* sock){
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    char msg[BUFFER_SIZE];
    int valread;

    if(!(*sock = socket(AF_INET,SOCK_STREAM,0))){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr) <= 0){
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if(connect(*sock,(struct sockaddr*)&addr,sizeof(addr)) < 0){
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

}

void client(){
    int sock;
    createclient(&sock);
    char msg[BUFFER_SIZE] = "exit";
    
    send(sock,msg,strlen(msg),0);
}

int main(){
    client();
    return 0;
}