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

    char buf[256];
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

    char computerName[64];
    printf("Computer name: ");
    scanf("%s", computerName);
    getchar();

    strcpy(buf, computerName);
    int pos = strlen(computerName);
    buf[pos] = '\0';
    pos++;

    int disksNum;
    printf("Number of disks: ");
    scanf("%d", &disksNum);
    getchar();

    char diskName;
    unsigned short int diskSize;

    for(int i = 0; i < disksNum; i++) {
        printf("Disk name: ");
        scanf("%c", &diskName);
        
        printf("Disk space: ");
        scanf("%hu", &diskSize);
        getchar();

        buf[pos] = diskName;
        pos++;
        memcpy(buf + pos, &diskSize, sizeof(diskSize));   
        pos += sizeof(diskSize);
    }
    // buf[pos + 1] = '\0';
    // printf("%s", buf);
    //send
        ret = send(client, buf, pos, 0);
        if(ret == -1) {
            perror("send() failed");
            return 1;
        }
        // fgets(buf, sizeof(buf), stdin);
        // buf[strcspn(buf, "\n")] = '\0';

        // send(client, buf, strlen(buf), 0);
    //close socket
    close(client);
}