#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

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
        // printf("Client IP: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    
        char *msg = "Hello";
        int ret = send(client, msg, strlen(msg), 0);
        if(ret == -1) {
            perror("send() failed!");
            continue;
        }

        //receive
        char *searchString = "0123456789";
        int len = strlen(searchString);
        bool check[len - 1];
        int count = 0;
        char buf[64];
        char tmpStr[16];
        strcpy(tmpStr, "");        

        while(1) {
            ret = recv(client, buf, sizeof(buf), 0);
            if(ret == -1) {
                perror("recv() failed");
                break;
            }else if(ret == 0) {
                break;
            }
            for(int i = 0; i < len - 1; i++) {
                if(check[i]) {
                    bool tmp;
                    for(int j = 0; j < len - 1 - i; j++) {
                        if(((int)buf[j] - 48) == (i + 1 + j)) {
                            tmp = true;
                        }else {
                            tmp = false;
                            break;
                        }
                    }
                    if(tmp) 
                        count ++;
                }
            }

            for(int i = 0; i < strlen(buf); i++) {
                if(strstr(&buf[i], searchString) == &buf[i]) {
                    count++;
                }
            }

            for(int i = 0; i < len - 1; i++) {
                for(int j = 0; j <= i; j++) {
                    if(((int)buf[sizeof(buf) - 1 - j]) == (j - j))
                        check[i] = true;
                    else {
                        check[i] = false;
                        break;
                    }
                }
            }
        }
        printf("So lan xuat hien cua xau %s: %d\n", searchString, count);
        close(client);
    }
    close(listener);

}