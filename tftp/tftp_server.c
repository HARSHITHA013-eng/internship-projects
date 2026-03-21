   #include "tftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet);

int main() 
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    tftp_packet packet;


    // Create UDP socket
    sockfd= socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        printf("Socket creation failed\n");
        exit(1);
    }


    // Set socket timeout option
    struct timeval tv;
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = 0;
    //TODO Use setsockopt() to set timeout option
    setsockopt(sockfd, SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));

    
    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if(bind(sockfd,(struct sockaddr *)&server_addr, sizeof(server_addr))<0)
    {
        printf("Bind failed\n");
        exit(1);
    }
    printf("TFTP Server listening on port %d...\n", PORT);

    // Main loop to handle incoming requests
    while (1) 
    {
        int n = recvfrom(sockfd, &packet, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (n < 0) {
            perror("Receive failed or timeout occurred");
            continue;
        }

         handle_client(sockfd, client_addr, client_len, &packet);
    }

    close(sockfd);
    return 0;
}

void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet) 
{
    //check opcode based on that mode open the file
    //wrq->create file in write mode
    //in write mode-> if created successfully send ack to client
    //rrq->check file is present or not if not present send error to client
    // Extract the TFTP operation (read or write) from the received packet
    // and call send_file or receive_file accordingly

    //extract opcode
    uint16_t opcode = ntohs(packet->opcode);
    if (opcode == WRQ)
    {
           printf("WRQ received for file %s\n",packet->body.request.filename);
           receive_file(sockfd,client_addr,client_len,packet->body.request.filename);   
    }
    else if(opcode ==RRQ)
    {
        printf("RRQ received for file %s\n",packet->body.request.filename);
        send_file(sockfd,client_addr,client_len,packet->body.request.filename);
    }
    
}




