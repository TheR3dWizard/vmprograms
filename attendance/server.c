#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>

#define PORT 8080
#define BUFFER_SIZE 1024



void createserver(int* serv_fd){
    struct sockaddr_in addr;
    int opt = 1;
    socklen_t addrlen = sizeof(addr);

    if(!(*serv_fd = socket(AF_INET,SOCK_STREAM,0))){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(*serv_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if(bind(*serv_fd,(struct sockaddr *)&addr,addrlen) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if(listen(*serv_fd,10) <0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
}


void server(){
    int serv_fd, new_sock, valread;
    struct sockaddr_in addr;
    int opt = 1;
    socklen_t addrlen = sizeof(addr);
    char buffer[BUFFER_SIZE] = {0};
    char msg[BUFFER_SIZE];
    char session[BUFFER_SIZE];

    printf("Enter session: ");
    scanf("%s",session);
    int len = strlen(session);
    strcpy(&session[len],".txt");
    FILE* fp = fopen(session,"w");

    createserver(&serv_fd);

    while(1){
        printf("Waiting for connection...\n");
        if((new_sock = accept(serv_fd,(struct sockaddr *)&addr,&addrlen)) < 0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        fp = fopen(session,"a");
        memset(buffer,0,BUFFER_SIZE);
        valread = read(new_sock,buffer,BUFFER_SIZE);
        if(strcmp(buffer,"exit") == 0){
            break;
        }
        fprintf(fp,"%s\n",buffer);
        printf("%s\n",buffer);
        close(new_sock);
        fclose(fp);
    }

}



int main(){
    
    server();
}