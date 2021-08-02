#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define PORT 20001

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main()
{
    int sockfd, n;
    char buffer[256];

    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname("hardik");
    if (server == NULL) {
        error("No host found");
    }

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    while(1)
    {
        printf("Please enter the message: ");
        bzero(buffer, 256);
        fgets(buffer,255,stdin);
        if(strncmp(buffer,"end",3)==0)
            break;

        n = send(sockfd, buffer, strlen(buffer), 0);
        if (n < 0)
            error("ERROR writing to socket");

        bzero(buffer, 256);

        n = recv(sockfd, buffer, 255, 0);
        if (n < 0)
            error("ERROR reading from socket");
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
