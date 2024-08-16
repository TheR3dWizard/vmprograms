#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

void searchDNS(char* url,char* ip){
    FILE* fp;
    char entry1[BUFFER_SIZE],entry2[BUFFER_SIZE];
    fp = fopen("dnsfile.txt","r");
    int flag = 0;
    while(!feof(fp)){
        fscanf(fp,"%s",entry1);
        fscanf(fp,"%s",entry2);
        //printf("URL:%s\nIP:%s\n",entry1,entry2);
        if(!strcmp(url,entry1)){
            strcpy(ip,entry2);
            flag = 1;
            break;
        }
    }
    if(!flag){
        strcpy(ip,"0");
    }
    fclose(fp);
}

int main(){
    //declarations
    char url[BUFFER_SIZE];
    char ip[BUFFER_SIZE];

    //get input
    printf("ENTER URL: ");
    scanf("%s",url);
    printf("ENTERED URL: %s\n",url);
    //search dns file
    searchDNS(url,ip);
    //return ip or not found
    if(!strcmp(ip,"0")) printf("IP NOT FOUND\n");
    else printf("IP ADDRESS: %s\n",ip);
}
