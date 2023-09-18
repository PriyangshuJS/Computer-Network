#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>	

struct sockaddr_in servadd;
unsigned short port_num=5012;
char serverIP[20]="172.18.177.212";
char rbuff[128];
char wbuff[128];
int main(){
	int sockFd=socket(AF_INET,SOCK_STREAM,0);
	if(sockFd<0){
		printf("SERVER:Cannot create socket\n");
		exit(1);
	}
	servadd.sin_family=AF_INET;
	servadd.sin_port=htons(port_num);
	inet_aton(serverIP,(&servadd.sin_addr));
	printf("\t Hello \n");
	
	int connfd=connect(sockFd,(struct sockaddr *)&servadd,sizeof(servadd));
	if(connfd<0){
		printf("Cannot connect the server sorry\n");
		close(sockFd);
		exit(1);
	}
	while(1){
		printf("Enter file name  : \n");
		int w;
		gets(wbuff);
			
		w=write(sockFd,wbuff,sizeof(wbuff));
		if((strcmp(wbuff,"Bye"))==0){
			exit(1);
		}
		if(w<0){
			printf("Cannot write data \n");
			close(sockFd);
			exit(1);
		}else{
		
			int r=read(sockFd,rbuff,128);
			if(r<0){
				printf("Cannot read data \n");
				close(sockFd);
				exit(1);
			}
			rbuff[r]='\0';
			printf("The message sent :  %s\n",rbuff);
		}
	
	}
	close(sockFd);
	exit(1);
}



