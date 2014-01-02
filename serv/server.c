#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include<sys/stat.h>
#define PORT 50000 
#define BACKLOG 5
#define LENGTH 512 
int ispresent(char* file){
struct stat st;
if(stat(file,&st) == 0)return 1;
return 0;
}
int main ()
{
int sockfd; 
int nsockfd; 
int num;
int sin_size; 
struct sockaddr_in addr_local; /* client addr */
struct sockaddr_in addr_remote; /* server addr */
char revbuf[LENGTH];

/* Get the Socket file descriptor */
if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
{
    fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
    exit(1);
}
else 
    printf("[Server] Obtaining socket descriptor successfully.\n");

/* Fill the client socket address struct */
addr_local.sin_family = AF_INET; // Protocol Family
addr_local.sin_port = htons(PORT); // Port number
addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct

/* Bind a special Port */
if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
{
    fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
    exit(1);
}
else 
    printf("[Server] Binded tcp port %d in addr 127.0.0.1 sucessfully.\n",PORT);

/* Listen remote connect/calling */
if(listen(sockfd,BACKLOG) == -1)
{
    fprintf(stderr, "ERROR: Failed to listen Port. (errno = %d)\n", errno);
    exit(1);
}
else
    printf ("[Server] Listening the port %d successfully.\n", PORT);

int success = 0;
while(success == 0)
{
    sin_size = sizeof(struct sockaddr_in);

    /* Wait a connection, and obtain a new socket file despriptor for single connection */
    if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
    {
        fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
        exit(1);
    }
    else 
        printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));


      /*      char buffer[256];
            bzero(buffer,256);
            int n = 0;
            n = read(nsockfd, buffer, 255);
            if (n < 0) error("ERROR reading from socket");
            printf("msg: %s\n",buffer);*/

    /*Receive File from Client */
    //char* fr_name = "recieve.txt";
    
char recv_data[1024];
int bytes_recieved = recv(nsockfd,recv_data,1024,0);

			recv_data[bytes_recieved] = '\0';

			if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
			{
				close(nsockfd);
				break;
			}

			else {
				char* fr_name=strtok (recv_data," ");
				char* fr_size = strtok (NULL," "); 
				printf("size:%s\n",fr_size);
				printf("file:%s size:%s\n",fr_name,fr_size);
				char re[10];
				scanf("%s",re);
				send(nsockfd, re,strlen(re), 0); 
if(!strcmp("yes",re)){
    FILE *fr = fopen(fr_name, "wb");
    if(fr == NULL)
        printf("File %s Cannot be opened file on server.\n", fr_name);
    else
    {
        bzero(revbuf, LENGTH); 
        int fr_block_sz = 0;
        while((fr_block_sz = recv(nsockfd, revbuf, LENGTH, 0)) > 0) 
        {
            int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
            if(write_sz < fr_block_sz)
            {
                error("File write failed on server.\n");
            }
            bzero(revbuf, LENGTH);
            if (fr_block_sz == 0 || fr_block_sz != 512) 
            {
                break;
            }
        }
        if(fr_block_sz < 0)
        {
            if (errno == EAGAIN)
            {
                printf("recv() timed out.\n");
            }
            else
            {
                fprintf(stderr, "recv() failed due to errno = %d\n", errno);
                exit(1);
            }
        }
        printf("Ok received from client!\n");
        fclose(fr); 
	
    }
}
}
bytes_recieved = recv(nsockfd,recv_data,1024,0);
recv_data[bytes_recieved] = '\0';
char str[15];
if(ispresent(recv_data)==0){
strcpy(str,"no");
send(nsockfd, str,strlen(str), 0);
}
else{ 
char sdbuf[1024];
strcpy(str,"yes");
send(nsockfd, str,strlen(str), 0);
FILE* fs=fopen(recv_data,"rb");
if(fs == NULL)
    {
        printf("ERROR: File %s not found.\n", recv_data);
        exit(1);
    }
int fs_block_sz;
    bzero(sdbuf, LENGTH); 
    while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
    {
        if(send(nsockfd, sdbuf, fs_block_sz, 0) < 0)
        {
            fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", recv_data, errno);
            break;
        }
        bzero(sdbuf, LENGTH);
    }
    printf("Ok File %s from Server was Sent!\n", recv_data);

}
close(nsockfd);
}
close(sockfd);
return 0;
}
