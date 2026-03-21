/*Name: harshitha G H
Date: 10-03-2026
Project Name: TFTP Server and Client(Trivial File Transfer Protocol) 
Description:  The objective of this project is to design and implement a simplified version of the Trivial File Transfer Protocol (TFTP) using the UDP protocol in the C programming language. The system allows a client to upload and download files from a server using standard TFTP operations such as Read Request (RRQ) and Write Request (WRQ).
*/


#include "tftp.h"
#include "tftp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>



int main() 
{
    char command[256];
    tftp_client_t client;
    memset(&client, 0, sizeof(client));  // Initialize client structure
    client.sockfd=-1; //not connected

    // Main loop for command-line interface
    while (1) {
        printf("tftp> ");
        fgets(command, sizeof(command), stdin);

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        // Process the command
        process_command(&client, command);
    }

    return 0;
}

// Function to process commands
void process_command(tftp_client_t *client, char *command) 
{
    int choice;
    char ip_addr[20];
    char filename[50];
    printf("------MENU------\n");
    printf("1.Connect\n2.Put\n3.Get\n4.Mode\n5.Exit\n");
    printf("----------------\n");
    printf("Enter the choice: \n");
    scanf("%d",&choice);
    getchar();
    switch(choice)
    {
        case 1:
        //connect
        printf("Enter the server ip address\n");
        fgets(ip_addr,sizeof(ip_addr),stdin);
        //remove new line
        ip_addr[strcspn(ip_addr,"\n")]=0;
        int len=strlen(ip_addr);
        int dot=0;
        for(int i=0; i<len; i++)
        {
            if(ip_addr[i]=='.')

            {
               dot++;
            }
        }
        if(dot!=3)
        {
            printf("Invalid ip address\n");
            return;
        }
        connect_to_server(client,ip_addr,PORT);
        printf("Connected to server\n");
        break;


        case 2:
        //put
        if(client->sockfd<0)
        {
            printf("Error: Use Connect first\n");
            return;
        }
        
        printf("Enter the filename to send:\n");
        fgets(filename,sizeof(filename),stdin);
        filename[strcspn(filename,"\n") ] =0;

        put_file(client,filename);
        break;

        case 3:
        //get
        if(client->sockfd<0)
        {
            printf("Error: Use connect first\n");
            return;
        }
        printf("Enter the filename to receive:\n");
        fgets(filename,sizeof(filename),stdin);
        filename[strcspn(filename,"\n") ] =0;

        char *ext = strrchr(filename,'.');

        if( (ext==NULL || strcmp(ext,".txt") ) !=0)
        {
            printf("Invalid filename\n");
            printf("Usage: [Filename.txt]\n");
            
        }

        get_file(client,filename);
        break;

        case 4:
        //mode

        case 5:
        //exit
        disconnect(client);
        exit(0);

        default:
        printf("Invalid input\n");




    }
   
}

// This function is to initialize socket with given server IP, no packets sent to server in this function
void connect_to_server(tftp_client_t *client, char *ip, int port) 
{
    // Create UDP socket
     client->sockfd=socket(AF_INET, SOCK_DGRAM,0);
     client->server_len = sizeof(client->server_addr);

    // Set socket timeout option
    /* struct timeval 
     {
           time_t       tv_sec;   // Seconds 
           suseconds_t  tv_usec;  // Microseconds 
     };
    */
    struct timeval tv;
    tv.tv_sec=10;
    tv.tv_usec=0;

    setsockopt(client->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    // Set up server address
    
    client->server_addr.sin_family=AF_INET;
    client->server_addr.sin_port=htons(port);
    client->server_addr.sin_addr.s_addr=inet_addr(ip);
   



}   
void put_file(tftp_client_t *client, char *filename)
{
   
    FILE *fp=fopen(filename,"rb");
    if(fp==NULL)
    {
        printf("Error : file does not exit\n");
        return;
    }
    printf("File found:Sending ...\n");
   // fclose(fp);
   tftp_packet packet;
    socklen_t addr_len = sizeof(client->server_addr);


    send_request(client->sockfd,  client->server_addr, filename, WRQ);
  //collect the ack and check success or failure
  int n=recvfrom(client->sockfd, &packet, BUFFER_SIZE, 0, (struct sockaddr *)&client->server_addr, &addr_len);
  if(n<0)
  {
    printf("Server did not respond\n");
    fclose(fp);
    return;
  } 
  printf("Ack received. Sending file\n");
  uint16_t block=1;
  size_t bytes;
  memset(&packet,0,sizeof(packet));
  fseek(fp,0,SEEK_SET);
  while(1)
  {
    memset(&packet,0,sizeof(packet));
    
    packet.opcode = htons(DATA);
    packet.body.data_packet.block_number = htons(block);
    bytes = fread(packet.body.data_packet.data,1,512,fp);
   
    
    char buffer[BUFFER_SIZE];

    uint16_t op = htons(DATA);
    uint16_t blk = htons(block);

    memcpy(buffer,&op,2);
    memcpy(buffer+2,&blk,2);
    memcpy(buffer+4,packet.body.data_packet.data,bytes);

    sendto(client->sockfd,buffer,bytes+4,0,(struct sockaddr *)&client->server_addr,sizeof(client->server_addr));
    printf("Sent block %d\n",block);
    n=recvfrom(client->sockfd,&packet,BUFFER_SIZE,0,(struct sockaddr *)&client->server_addr, &addr_len);
    if(n<0)
    {
        printf("ACK timeout\n");
        break;
    }
    if(ntohs(packet.opcode)!= ACK)
    {
        printf("ACK not received. Resending block %d\n",block);
        continue;
    }
    printf("ACK received for block %d\n",block);
    block++;
    if(bytes<512)
    {
        break;
    }
  }
  printf("File transfer completed\n");
  fclose(fp);

 //read 512 bytes of data from the file
 // send 512 bytes of data
 //if failure stop and print menu   
 //if succcess send next 512 bytes if failure send same 512 bytes 
 //based on mode send data in bytes 512 or 1byte->8 bits
 //inside while loop read data until end of file 
 //inside while update struct variable


    

}

void get_file(tftp_client_t *client, char *filename)
{
    //send rrq
    send_request(client->sockfd,client->server_addr,filename,RRQ);
    printf("RRQ sent.... Waiting for data\n");
    //receive file
    receive_request(client->sockfd,client->server_addr,filename,RRQ);

  
}

void disconnect(tftp_client_t *client) 
{
    // close fd
    if(client->sockfd>=0)
    {
        close(client->sockfd);
        client->sockfd=-1;
        printf("Disconnected from server\n");
    }
   
}
void send_request(int sockfd, struct sockaddr_in server_addr, char *filename, int opcode)
{
    char buff[BUFFER_SIZE];
    int len=0;

    uint16_t op= htons(opcode);

    //copy opcode-2bytes
    memcpy(buff, &op,2);
    len=len+2;

    //copy filename
    strcpy(buff+len,filename);
    len = len+strlen(filename)+1;

    //copy mode
    strcpy(buff+len,"octet");
    len= len+strlen("octet")+1;

    //send packet
    sendto(sockfd,buff,len,0,(struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Request sent to server\n");


}

void receive_request(int sockfd, struct sockaddr_in server_addr, char *filename, int opcode)
{
    tftp_packet packet;
    socklen_t addr_len = sizeof(server_addr);
    //store receive data
    FILE *fp = fopen(filename,"wb");
    if(fp==NULL)
    {
        printf("Error creating file\n");
        return;
    }
    printf("Receiving file\n");
    while(1)
    {
        int n= recvfrom(sockfd,&packet,BUFFER_SIZE,0,(struct sockaddr *)&server_addr,&addr_len);
        if(n<0)
        {
            printf("Error receiving data\n");
            break;
        }
        //packet is data or not
        if(ntohs(packet.opcode)!=DATA)
        {
            break;
        }
        //extract block no
        int block = ntohs(packet.body.data_packet.block_number);
        //write to file
        fwrite(packet.body.data_packet.data,1,n-4,fp);
        printf("Received block %d\n",block);
        //ack packet
        packet.opcode = htons(ACK);
        packet.body.ack_packet.block_number = htons(block);
        //send ack to server
        sendto(sockfd,&packet,4,0,(struct sockaddr *)&server_addr,addr_len);
        if(n<BUFFER_SIZE)
        {
            break;
        }



    }
    fclose(fp);
    printf("File received successfully\n");
    
}