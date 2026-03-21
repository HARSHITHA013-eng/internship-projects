/* Common file for server & client */

#include "tftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void send_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename) 
{
    // Implement file sending logic here(RRQ)
    FILE *fp= fopen(filename,"rb");
    if(fp== NULL)
    {
        printf("File not found\n");
        return;
    }
    printf("Sending file: %s\n",filename);
    //create packet structure for sending data
    tftp_packet packet;
    //data packet block number=1
    int block=1;
    //variable to store number of bytes read from file
    size_t bytes;
    while( (bytes = fread(packet.body.data_packet.data,1,512,fp)) >0)
    {
        memset(&packet,0,sizeof(packet));
        //set opcode as data packet
        packet.opcode = htons(DATA);
        //set block no for packet
        packet.body.data_packet.block_number = htons(block);
        //sent to client
        sendto(sockfd,&packet,bytes +4,0,(struct sockaddr *)&client_addr,client_len);
        printf("Sent block %d\n",block);
        //wait for ack from client
        int n= recvfrom(sockfd,&packet,BUFFER_SIZE,0,(struct sockaddr *)&client_addr,&client_len);
        if(n<0)
        {
            printf("Error receiving ACK\n");
            break;

        }
        //received ack or not
        if(ntohs(packet.opcode)!=ACK)
        {
            printf("Expected ack but received something else\n");
            break;
        }
        //extract ack block number
        int ack_block=ntohs(packet.body.ack_packet.block_number);
        //received ack or not
        printf("ACk received for block %d\n",ack_block);
        block++;
        if(bytes<512)
        {
            break;
        }


    }
    fclose(fp);
    printf("File sent successfully\n");
    
}

void receive_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename) 
{
    // Implement file receiving logic here(WRQ)
    FILE *fp= fopen(filename,"wb");
    if(fp== NULL)
    {
        printf("Error in creating the file\n");
        return;
    }
    printf("Receiving file: %s\n",filename);
    tftp_packet packet;
    //send ACK block 0
    packet.opcode= htons(ACK);
    //first ack block number is always 0
    packet.body.ack_packet.block_number = htons(0);
    //send ack to client
    sendto(sockfd, &packet,4,0, (struct sockaddr *)&client_addr, client_len);
    while(1)
  {
    char buffer[BUFFER_SIZE];
    
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                     (struct sockaddr *)&client_addr, &client_len);
    //receive packet from client
        if(n<=0)
        {
            continue;
        }
    if(n < 4)
    {
        printf("Invalid packet\n");
        continue;
    }
    //minimum packet size check

    uint16_t opcode;
    uint16_t block;

    memcpy(&opcode, buffer, 2);
    opcode = ntohs(opcode);

    if(opcode != DATA)
    {
        continue;
    }

    memcpy(&block, buffer+2, 2);
    block = ntohs(block);
    //extract block number

    fwrite(buffer+4,1,n-4,fp);
    //write received data into file

    printf("Received block %d\n",block);

    //prepare ACK
    tftp_packet packet;
    packet.opcode = htons(ACK);
    packet.body.ack_packet.block_number = htons(block);
    //send ACK back to client
    sendto(sockfd,&packet,4,0,(struct sockaddr *)&client_addr,client_len);
    
        //last packet condition
    if(n < BUFFER_SIZE)
    {
        break;
    }
    
  }
    fclose(fp);
    printf("File received successfully");
}