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

#define LENGTH 512
#define BACKLOG 5
#define SERVER_PORT 6000
#define CLIENT_PORT 5000

int ispresent(char* file){
	struct stat st;
	if(stat(file,&st) == 0)return 1;
	return 0;
}
off_t fsize(const char *filename) {
	struct stat st; 

	if (stat(filename, &st) == 0)
		return st.st_size;

	fprintf(stderr, "Cannot determine size of %s: %s\n",
			filename, strerror(errno));

	return -1; 
}


void upload(int* nsock,char* filename){
	int nsockfd=*nsock;
	char file_size[20];
	int file=recv(nsockfd, file_size, LENGTH, 0);
		printf("file_size: %s\n",file_size);
	char judge[15];
	scanf("%s",judge);
	if(send(nsockfd, judge, strlen(judge), 0)==-1)perror("Aman1");
	if(!strcmp(judge,"yes")){
		FILE *fr = fopen(filename, "wb");
		if(fr == NULL)
			printf("File %s Cannot be opened file on client.\n", filename);
		else
		{	char revbuf[LENGTH];
			bzero(revbuf, LENGTH); 
			int fr_block_sz = 0;
			while((fr_block_sz = recv(nsockfd, revbuf, LENGTH, 0)) > 0) 
			{
				int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
				if(write_sz < fr_block_sz)
				{
					error("File write failed on client.\n");
				}
				if(revbuf[0]==0)break;	
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
			printf("Ok received %s from server!\n",filename);

		}
		fclose(fr);
	}
close(nsockfd);
	return;
}

void download(){
	printf("download\n");
	/*if(send(nsockfd, filename, LENGTH, 0)==-1)perror("Aman1");
	printf("filename:%s\n",filename);
	char recv_data[LENGTH];
	int recv_size;
	recv_size=recv(nsockfd, recv_data, LENGTH, 0);
	recv_data[recv_size]=0;
	printf("filesize:%s\n",recv_data);
	recv_size=recv(nsockfd, recv_data, LENGTH, 0);
	recv_data[recv_size]=0;
	printf("Md5hash:%s\n",recv_data);
	recv_size=recv(nsockfd, recv_data, LENGTH, 0);
	recv_data[recv_size]=0;
	printf("TimeStamp:%s\n",recv_data);
*/
	return;
}

void index_short(char* str3,char* str4){
	printf("short\n");return;
}

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
				if(send(nsockfd, t, LENGTH, 0)==-1)
					perror("Aman1");
				//	send(nsockfd, t, strlen(t), 0);
			}
			if(count==9){send(nsockfd, "\n", LENGTH, 0);count=0;}
		}
		send(nsockfd,"",LENGTH,0);
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
				//	send(nsockfd, t, strlen(t), 0);
				if(send(nsockfd, t, LENGTH, 0)==-1)
					perror("Aman1");
			}
			if(count==9){send(nsockfd, "\n", LENGTH, 0);count=0;}
		}
		send(nsockfd,"",LENGTH,0);
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
			//send(nsockfd, t, strlen(t), 0);
			if(send(nsockfd, t, LENGTH, 0)==-1)
				perror("Aman1");
		}
		if(count==9){
			fscanf(fp1,"%s",t);
			strcat(t," ");
			//send(nsockfd, t, strlen(t), 0);
			if(send(nsockfd, t, LENGTH, 0)==-1)
				perror("Aman1");
			fscanf(fp1,"%s",t);
			send(nsockfd, "\n", LENGTH, 0);count=0;
		}
	}
	send(nsockfd,"",LENGTH,0);
	while(fscanf(fp1,"%s",t)!=EOF);
	fclose(fp);
	fclose(fp1);
	system("rm -rf out.txt");
	system("rm -rf out1.txt");
	return;
}
void checkall(int *nsock){
	printf("msin ghus gaya\n");
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
			//if(send(nsockfd, t, strlen(t), 0)==-1)
			if(send(nsockfd, t, LENGTH, 0)==-1)
				perror("Aman1");
		}
		if(count==9){
			fscanf(fp1,"%s",t1);
			strcat(t1," ");
			//if(send(nsockfd, t1, strlen(t1), 0)==-1)
			if(send(nsockfd, t1, LENGTH, 0)==-1)
				perror("Aman2");
			fscanf(fp1,"%s",t1);
			strcat(t1," ");
			if(send(nsockfd, "\n", LENGTH, 0)==-1)
				perror("Aman3");
			count=0;
		}
	}
	send(nsockfd,"",LENGTH,0);
	while(fscanf(fp1,"%s",t1)!=EOF);
	fclose(fp);
	fclose(fp1);
	system("rm -rf out.txt");
	system("rm -rf out1.txt");
	return;
}
int main (int argc, char *argv[])
{
	if(argc != 1){
		printf("Usage : [exec] [ip-address]\n");
		return(1);
	}
	int pid=fork();
	if(pid<0){
		fprintf(stderr,"Error in creating Fork %d\n",errno);
		exit(-1);
	}
	if(pid==0){

		printf("Child\n");
		//client
		int sockfd; 
		int nsockfd;
		char revbuf[LENGTH]; 
		struct sockaddr_in remote_addr;
		int slen=sizeof(remote_addr);
		/* Get the Socket file descriptor */
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
			exit(1);
		}

		/* Fill the socket address struct */
		remote_addr.sin_family = AF_INET; 
		remote_addr.sin_port = htons(CLIENT_PORT); 
		char ip[100]="127.0.0.1";
		inet_pton(AF_INET, ip, &remote_addr.sin_addr); 
		//inet_pton(AF_INET, "10.1.39.21", &remote_addr.sin_addr); 
		//inet_pton(AF_INET, "10.1.39.96", &remote_addr.sin_addr); 
		bzero(&(remote_addr.sin_zero), 8);
		char sdbuf[LENGTH];
		/* Try to connect the remote */
		while(connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
		{
			//fprintf(stderr, "[Client] Waiting for server at port 50000\n");
		}
		printf("[Client] Connected to server at %s at port 5000...ok!\n",ip);
		while(1){
			char query[100];
			printf("Enter query\n");
			gets(query);	
			//scanf("%[^\n]",query);
			send(sockfd,query,strlen(query), 0); 
			char * pch;
			pch = strtok (query," ");
			if(!strcmp("FileUpload",pch)){
				pch = strtok (NULL," ");
				char str[15];
				int fs_size=fsize(pch);
				sprintf(str, "%d", fs_size);
				send(sockfd,str,LENGTH,0);
				int fr_block_sz = recv(sockfd,str, LENGTH, 0);
				str[fr_block_sz]=0;
				int fs_block_sz;
					if(!strcmp("yes",str)){	
							FILE* fs=fopen(pch,"rb");	
							char sdbuf[LENGTH];
							bzero(sdbuf, LENGTH); 
							while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
							{
							if(send(sockfd, sdbuf, LENGTH, 0) < 0)
							{
							fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", pch, errno);
							break;
							}
							bzero(sdbuf, LENGTH);
							}
							fclose(fs);
							}
							}		
							//FILE *fr = fopen("response.txt", "wb");
							//if(fr == NULL)
							//	printf("File response.txt Cannot be opened file on client.\n");
							//else
							{	char revbuf[LENGTH];
							bzero(revbuf, LENGTH); 
							int fr_block_sz = 0;
							//while((fr_block_sz = recvfrom(sockfd, revbuf, LENGTH, 0,&remote_addr,&slen)) > 0) 
							while((fr_block_sz = recv(sockfd, revbuf, LENGTH, 0)) > 0) 
							{
								slen=sizeof(remote_addr);
								revbuf[fr_block_sz]=0;
								if(revbuf[0]==0)
									break;
								printf("%s",revbuf);
								/*int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
								  if(write_sz < fr_block_sz)
								  {
								  error("File write failed on client.\n");
								  }*/
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
									//perror("Aman");
									exit(1);
								}
							}
							printf("Ok received from server!\n");

							}
							//fclose(fr); 
		}
		close(sockfd);
	}
	else{
		int status;
		//int client_pid=wait(&status);
		int sockfd; 
		int nsockfd; 
		int num;
		int sin_size; 
		struct sockaddr_in addr_local; /* client addr */
		struct sockaddr_in addr_remote; /* server addr */
		char revbuf[LENGTH];

		printf("Parent\n");
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
		addr_local.sin_port = htons(SERVER_PORT); // Port number
		addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
		bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct

		/* Bind a special Port */
		if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
		{
			fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
			exit(1);
		}
		else 
			printf("[Server] Binded tcp port 6000 in addr 127.0.0.1 sucessfully.\n");

		/* Listen remote connect/calling */
		if(listen(sockfd,BACKLOG) == -1)
		{
			fprintf(stderr, "ERROR: Failed to listen Port. (errno = %d)\n", errno);
			exit(1);
		}
		else
			printf ("[Server] Listening the port 6000 successfully.\n");

		/* Wait a connection, and obtain a new socket file despriptor for single connection */
		if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
		{
			fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
			exit(1);
		}
		else 
			printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));

		int success = 0;
		while(success == 0)
		{
			sin_size = sizeof(struct sockaddr_in);

			/* Wait a connection, and obtain a new socket file despriptor for single connection */
			/*if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
			  {
			  fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
			  exit(1);
			  }
			  else 
			  printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));
			  */
			char recv_data[1024];
			int bytes_recieved = recv(nsockfd,recv_data,1024,0);

			recv_data[bytes_recieved] = '\0';

			if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
			{
				close(nsockfd);
				break;
			}
			else{
				//	printf("recv:%s\n",recv_data);
				char * pch;
				char str1[500],str2[500],str3[500],str4[500];
				pch = strtok (recv_data," ");
				if(pch!=NULL){
					strcpy(str1,pch);
					printf("str1: %s\n",str1);
					pch = strtok (NULL, " ");
				}
				if(pch!=NULL){
					strcpy(str2,pch);
					printf("str1: %s\n",str2);
					pch = strtok (NULL, " ");
				}
				if(pch!=NULL){
					strcpy(str3,pch);
					printf("str1: %s\n",str3);
					pch = strtok (NULL, " ");
				}
				if(pch!=NULL){
					strcpy(str4,pch);
					printf("str1: %s\n",str4);
					pch = strtok (NULL, " ");
				}

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
					download(&nsockfd,str2);
				}
				if(!strcmp(str1,"FileUpload")){
					upload(&nsockfd,str2);
				}
			}
			//close(nsockfd);
		}
		close(nsockfd);
		close(sockfd);
	}
	return 0;
}
