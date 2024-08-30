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
	int i = 0;
	int serv_fd,new_sock,rec,check;
	createserver(&serv_fd);
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	char buffer[BUFFER_SIZE],msg[BUFFER_SIZE],ack[BUFFER_SIZE];
    strcpy(ack,"ACK");
	if((new_sock = accept(serv_fd,(struct sockaddr *)&addr,&addrlen)) < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("WORKS");
    while(1){
    	read(new_sock,msg,BUFFER_SIZE);
    	rec = atoi(msg);
    	if(rec == i+1){
    		printf("Recieved message %d",i);
    		send(new_sock,ack,BUFFER_SIZE,0);
    		i++;
    	}
    }
}

int main(){

	server();
}
