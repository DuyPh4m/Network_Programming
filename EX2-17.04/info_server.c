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
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1) {
        perror("socket() failed");
        return 1;
    }

    // server
    int portNum = 9001;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(portNum);

    //bind
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind() failed!\n");
        return 1;
    }
    
    //listen
    int backlog = 5;
    if(listen(listener, backlog) == -1) {
        perror("listen() failed!");
        return 1;
    }
    printf("Waiting for clients...\n");
    
    //accept
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    while(1) {  
        int client = accept(listener, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if(client == -1) {
            perror("accept() failed!");
            return 1;
        }
        printf("Client IP: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        
        
        
        char *msg = "Hello";
        int ret = send(client, msg, strlen(msg), 0);
        if(ret == -1) {
            perror("send() failed!");
            continue;
        }
            char buf[256];
            ret = recv(client, buf, sizeof(buf), 0);
            if(ret < sizeof(buf))
                buf[ret] = '\0';
            
            int pos = 0;
            char computerName[64];
            strcpy(computerName, buf);
            pos += strlen(computerName) + 1;\

            printf("Computer name: %s\n", computerName);

            int disksNum = (ret - pos) / 3;

            for(int i = 0; i < disksNum; i++) {
                char diskName;
                unsigned short int diskSize;

                diskName = buf[pos];
                pos++;

                memcpy(&diskSize, buf + pos, sizeof(diskSize));
                pos+= sizeof(diskSize);

                printf("%c: %hu\n", diskName, diskSize);
            }

            
        close(client);
    }
    close(listener);

}