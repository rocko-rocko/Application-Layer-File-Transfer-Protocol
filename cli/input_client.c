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


int main(){
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
char sdbuf[LENGTH];
/* Try to connect the remote */
if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
{
    fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);
    exit(1);
}
else 
    printf("[Client] Connected to server at port %d...ok!\n", PORT);

char query[100];
scanf("%[^\n]",query);
send(sockfd,query,strlen(query), 0); 
FILE *fr = fopen("response", "wb");
    if(fr == NULL)
        printf("File response Cannot be opened file on client.\n");
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

close(sockfd);
return 0;
}




