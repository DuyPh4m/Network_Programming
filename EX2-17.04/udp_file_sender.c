#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *ip_address = argv[1];
    int portNumber = atoi(argv[2]);
    char fileName[30] ;
    char endFile[4];
    strcpy(endFile, "---");

    FILE *fp;

    printf("File name: ");
    scanf("%s", fileName);
    getchar();

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("Wrong file name!!!\n");
        return 0;
    }

    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_address);
    addr.sin_port = htons(portNumber);

    char buf[256];

    size_t ret = sendto(sender, fileName, sizeof(fileName), 0,(struct sockaddr *)&addr, sizeof(addr));

    while(!feof(fp)) {
        size_t msg = fread (buf, 1, sizeof(buf), fp);
        if(msg <= 0) 
            break;
        ret = sendto(sender, buf, msg, 0, (struct sockaddr *) &addr, sizeof(addr));
    }
    ret = sendto(sender, endFile, sizeof(endFile), 0, (struct sockaddr *)&addr, sizeof(addr));
    fclose(fp);
}