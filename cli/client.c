#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define PORT 50000
#define LENGTH 512 

off_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

    return -1;
}
int main(int argc, char *argv[]){
int sockfd; 
int nsockfd;
char revbuf[LENGTH]; 
struct sockaddr_in remote_addr;

/* Get the Socket file descriptor */
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
{
    fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
    exit(1);
}

/* Fill the socket address struct */
remote_addr.sin_family = AF_INET; 
remote_addr.sin_port = htons(PORT); 
inet_pton(AF_INET, "127.0.0.1", &remote_addr.sin_addr); 
//inet_pton(AF_INET, "10.1.39.21", &remote_addr.sin_addr); 
//inet_pton(AF_INET, "10.1.39.96", &remote_addr.sin_addr); 
bzero(&(remote_addr.sin_zero), 8);

/* Try to connect the remote */
if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
{
    fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);
    exit(1);
}
else 
    printf("[Client] Connected to server at port %d...ok!\n", PORT);

/* Send File to Server */
//if(!fork())
//{
    char fs_name[100];
	scanf("%s",fs_name);
    char sdbuf[LENGTH]; 

/* char buffer[256];
bzero(buffer,256);
int n = 0;
n = read(nsockfd, buffer, 255);
if (n < 0) error("ERROR reading from socket");
printf("msg: %s\n",buffer);*/

    printf("[Client] Sending %s to the Server... ", fs_name);
    FILE *fs = fopen(fs_name, "rb");
    int fs_size=(int)fsize(fs_name);
    printf("file:%d\n",fs_size);
    char str[15];
    sprintf(str, "%d", fs_size);			
    char send_data[1024],recv_data[1024];
int 	bytes_recieved;
    strcpy(send_data,fs_name);
    strcat(send_data," ");
    strcat(send_data,str);
		if (strcmp(send_data , "q") != 0 && strcmp(send_data , "Q") != 0)
			send(sockfd,send_data,strlen(send_data), 0); 

		else
		{
			send(sockfd,send_data,strlen(send_data), 0);   
			close(sockfd);
			//break;
		}

		bytes_recieved=recv(sockfd,recv_data,1024,0);
		recv_data[bytes_recieved] = '\0';

		if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
		{	
			close(sockfd);
			//break;
		}

		else{
		   if(!strcmp("yes",recv_data)){

    if(fs == NULL)
    {
        printf("ERROR: File %s not found.\n", fs_name);
        exit(1);
    }
int fs_block_sz;
    bzero(sdbuf, LENGTH); 
    while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
    {
        if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
        {
            fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
            break;
        }
        bzero(sdbuf, LENGTH);
    }
    printf("Ok File %s from Client was Sent!\n", fs_name);
//}


}
printf("Enter file to download\n");
char download[1024],recv1_data[1024];
scanf("%[^\n]",download);
send(sockfd,download,strlen(download), 0); 
strcpy(recv1_data,"");
bytes_recieved=recv(sockfd,recv1_data,3,0);
printf("recvbytes:%d\n",bytes_recieved);
recv1_data[bytes_recieved] = '\0';
printf("recv:%s\n",recv1_data);
if(!strcmp(recv1_data,"yes")){
 FILE *fr = fopen(download, "wb");
    if(fr == NULL)
        printf("File %s Cannot be opened file on client.\n", download);
    else
    {	char revbuf[LENGTH];
        bzero(revbuf, LENGTH); 
        int fr_block_sz = 0;
        while((fr_block_sz = recv(sockfd, revbuf, LENGTH, 0)) > 0) 
        {
            int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
            if(write_sz < fr_block_sz)
            {
                error("File write failed on client.\n");
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
        printf("Ok received from server!\n");
	
    }
        fclose(fr); 

}
}
close (sockfd);
printf("[Client] Connection lost.\n");
return 0;
}
