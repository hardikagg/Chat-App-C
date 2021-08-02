#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 20001

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
     int sockfd, newsockfd, n;
     char buffer[256];

     socklen_t clilen;

     struct sockaddr_in serv_addr, cli_addr;

     sockfd =  socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORT);

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
              error("ERROR on binding");

     listen(sockfd,5);

     clilen = sizeof(cli_addr);

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");

     printf("server: got connection from %s port %d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

     while(1)
     {
         bzero(buffer, 256);

         n=recv(newsockfd, buffer, 255, 0);
         if (n < 0)
            error("ERROR reading from socket");

         printf("Client: %s",buffer);
         bzero(buffer, 256);

         printf("\nEnter data to be send to client: ");
         fgets(buffer,255,stdin);
         if(strncmp(buffer,"end",3)==0)
            break;
         n=send(newsockfd,buffer,255,0);
         if(n<0)
            error("Error in sending");
     }

     close(newsockfd);
     close(sockfd);
     return 0;
}
