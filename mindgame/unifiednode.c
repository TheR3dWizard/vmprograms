#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>


#define PORT 8080
#define BUFFER_SIZE 1024
void client(char* id){
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
    }else{
        send(sock,id,strlen(id),0);
    }

    while(1){
        printf("Enter message: ");
        scanf("%s",msg);
        send(sock,msg,strlen(msg),0);
    }
}

void server(){
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

    printf("Waiting for incoming message...\n\n");

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

    fd_set master_set, read_fds;
    int fd_max, new_socket, client_socket[30], activity, i;
    int sock_ids[30],count = 0;
    FD_ZERO(&master_set);
    FD_SET(serv_fd, &master_set);
    fd_max = serv_fd;

    while(1) {
        read_fds = master_set;
        activity = select(fd_max + 1, &read_fds, NULL, NULL, NULL);

        if (FD_ISSET(serv_fd, &read_fds)) {
            new_socket = accept(serv_fd, NULL, NULL);
            sock_ids[count] = new_socket;
            count++;
            printf("New client connected\n");
            FD_SET(new_socket, &master_set);
            if (new_socket > fd_max) fd_max = new_socket;

        }

        for (i = 0; i <= count; i++) {
            if (FD_ISSET(sock_ids[i], &read_fds)) {
                if ((valread = read(sock_ids[i], buffer, 1024)) == 0) {
                    printf("Client %d disconnected\n", i);
                    close(sock_ids[i]);
                    FD_CLR(sock_ids[i], &master_set);
                } else {
                    buffer[valread] = '\0';
                    printf("Client %d: %s\n", i, buffer);
                }
            }
        }
    }

}



int main(){
    char choice,msg[1024];
    printf("Enter 's' for server and 'c' for client: ");
    scanf("%c",&choice);
    if(choice == 's'){
        server();
    }else if(choice == 'c'){
        client("1");
    }else{
        printf("Invalid choice\n");
    }
}