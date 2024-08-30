#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void createclient(int* sock){
    struct sockaddr_in addr;
    
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

void client(int n){
	int sock,valread;
	char buffer[BUFFER_SIZE],msg[BUFFER_SIZE];
	
	createclient(&sock);

	
	int i = 0;
	while(i < n){
		printf("Sending packet number %d",i);
		sprintf(buffer,"%d",i);
		send(sock,buffer,strlen(buffer),0);
		read(sock,msg,BUFFER_SIZE);
		if(!strcmp(msg,"ACK")){
			i++;
		}
	}
}

int main(){
	int n;
	char msg[100];
	printf("Enter number of packets to send: ");
	printf("WORKS");
	scanf("%s ",msg);
	n = atoi(msg);
	client(n);
}

























