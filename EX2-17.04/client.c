#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // tao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client == -1) {
        perror("socket() failed");
        return 1;
    }

    // server
    char *serverIP = "127.0.0.1";
    int portNum = 9001;
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(serverIP);
    addr.sin_port = htons(portNum);

    // connect
    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) {
        perror("connect() failed");
        // return 1;
    }

    char buf[64];
    // receive
    ret = recv(client, buf, sizeof(buf), 0);
    if(ret == -1) {
        perror("recv() failed");
        // return 1;
    }else if(ret == 0) {
        printf("Connection closed!\n");
        // return 1;
    }

    if(ret < sizeof(buf))
        buf[ret] = '\0';
    printf("%s\n", buf);

    //send
    char *fileName = "text.txt";
    FILE *fp = fopen(fileName, "r");
    if(fp == NULL) {
        perror("fopen() failed");
        return 1;
    }
    
    while(!feof(fp)) {
        size_t msg = fread(buf, 1, sizeof(buf), fp);
        if(msg <= 0)
            break;
        send(client, buf, msg, 0);
    }
    fclose(fp);
        
    //close socket
    close(client);
}