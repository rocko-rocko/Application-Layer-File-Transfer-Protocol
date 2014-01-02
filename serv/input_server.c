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
void download(){
printf("download\n");
return;
}
void upload(){
printf("upload\n");
return;
}

void index_short(char* str3,char* str4){printf("short\n");return;}
void index_long(int* nsock){
int nsockfd=*nsock;
if(system("ls -alt > out.txt")==-1)printf("Command cant be executed\n");
char sdbuf[1024];
FILE* fp=fopen("out.txt","rb");
if(fp == NULL)
    {
        printf("ERROR: File out.txt not found.\n");
        return;
    }
else{
char t[200];
fscanf(fp,"%s",t);
fscanf(fp,"%s",t);
int count=0;
while(fscanf(fp,"%s",t)!=EOF){
count++;
if(count>=5 && count<=9){
strcat(t," ");
send(nsockfd, t, strlen(t), 0);
}
if(count==9){send(nsockfd, "\n", strlen("\n"), 0);count=0;}
}
fclose(fp);
system("rm -rf out.txt");
return;
}
}
void index_reg(int* nsock,char* str){
int nsockfd=*nsock;
char t[200]="ls -lt ";
strcat(t,str);
strcat(t," >out.txt");
if(system(t)==-1)printf("Command cant be executed\n");
FILE *fp=fopen("out.txt","rb");
if(fp == NULL)
    {
        printf("ERROR: File out.txt not found.\n");
        return;
    }
else{
int count=0;
while(fscanf(fp,"%s",t)!=EOF){
count++;
if(count>=5 && count<=9)
{
strcat(t," ");
send(nsockfd, t, strlen(t), 0);
}
if(count==9){send(nsockfd, "\n", strlen("\n"), 0);count=0;}
}
fclose(fp);
system("rm -rf out.txt");
}
return;
}
void verify(int* nsock,char* str){
int nsockfd=*nsock;
char cmd1[200]="ls -l ";
char cmd2[200]="md5sum ";
int count=0;

strcat(cmd1,str);
strcat(cmd2,str);
strcat(cmd1," >out.txt");
strcat(cmd2," >out1.txt");
system(cmd1);
system(cmd2);
FILE* fp=fopen("out.txt","rb");
FILE* fp1=fopen("out1.txt","rb");
char t[200];
while(fscanf(fp,"%s",t)!=EOF){
count++;
if(count>=6 && count<=9){
strcat(t," ");
send(nsockfd, t, strlen(t), 0);}
if(count==9){
fscanf(fp1,"%s",t);
strcat(t," ");
send(nsockfd, t, strlen(t), 0);
fscanf(fp1,"%s",t);
send(nsockfd, "\n", strlen("\n"), 0);count=0;
}
}
while(fscanf(fp1,"%s",t)!=EOF);
fclose(fp);
fclose(fp1);
system("rm -rf out.txt");
system("rm -rf out1.txt");
return;
}
void checkall(int *nsock){
printf("\nhi\n");
int nsockfd=*nsock;
if(system("ls -l > out.txt")==-1)printf("Command cant be executed\n");
if(system("md5sum `ls` > out1.txt")==-1)printf("hash cant get");
FILE *fp=fopen("out.txt","rw");
FILE *fp1=fopen("out1.txt","rw");
char t[200];
char t1[200];
fscanf(fp,"%s",t);
fscanf(fp,"%s",t);
int count=0;
while(fscanf(fp,"%s",t)!=EOF){
count++;
if(count>=6 && count<=9){
strcat(t," ");
send(nsockfd, t, strlen(t), 0);}
if(count==9){
fscanf(fp1,"%s",t1);
strcat(t1," ");
send(nsockfd, t1, strlen(t1), 0);
fscanf(fp1,"%s",t1);
strcat(t1," ");
send(nsockfd, "\n", strlen("\n"), 0);count=0;
}
}
while(fscanf(fp1,"%s",t1)!=EOF);
fclose(fp);
fclose(fp1);
system("rm -rf out.txt");
system("rm -rf out1.txt");
return;
}

int main()
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
else{
printf("recv:%s\n",recv_data);
char * pch;
char str1[500],str2[500],str3[500],str4[500];
pch = strtok (recv_data," ");
if(pch!=NULL){
strcpy(str1,pch);
pch = strtok (NULL, " ");
}
if(pch!=NULL){
strcpy(str2,pch);
pch = strtok (NULL, " ");
}
if(pch!=NULL){
strcpy(str3,pch);
pch = strtok (NULL, " ");
}
if(pch!=NULL){
strcpy(str4,pch);
pch = strtok (NULL, " ");
}
 
/*char str1[500];
scanf("%s",str1);
char str2[500];
scanf("%s",str2);
char str3[500];
if(strcmp(str2,"LongList")!=0 && strcmp(str2,"CheckAll")!=0 && strcmp(str1,"FileDownload")!=0 && strcmp(str1,"FileUpload")!=0)scanf("%s",str3);*/
if(!strcmp(str1,"IndexGet")){
if(!strcmp(str2,"ShortList")){
/*char str4[500];
scanf("%s",str4);*/
index_short(str3,str4);
}
if(!strcmp(str2,"LongList")){index_long(&nsockfd);}
if(!strcmp(str2,"RegEx")){index_reg(&nsockfd,str3);}
}
if(!strcmp(str1,"FileHash")){
if(!strcmp(str2,"Verify")){
verify(&nsockfd,str3);
}
if(!strcmp(str2,"CheckAll")){checkall(&nsockfd);}
}
if(!strcmp(str1,"FileDownload")){
download(str2);
}
if(!strcmp(str1,"FileUpload")){
upload(str2);
}
}
close(nsockfd);
}
close(sockfd);
return 0;
}





