#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include <dirent.h>
#include <errno.h>	

struct sockaddr_in servadd,clientAdd;
unsigned short port_num=5012;
char serverIP[]="172.18.177.212";
char rbuff[128];
int main(){
	int sockFd=socket(AF_INET,SOCK_STREAM,0);
	if(sockFd<0){
		printf("SERVER:Cannot create socket\n");
		exit(1);
	}
	servadd.sin_family=AF_INET;
	servadd.sin_port=htons(port_num);
	inet_aton(serverIP,(&servadd.sin_addr));
	printf("\t Server is Running... \n");
	
	int bindFun=bind(sockFd,(struct sockaddr *)&servadd,sizeof(servadd));
	if(bindFun<0){
		printf("Server:cannot bind the address\n");
		close(sockFd);
		exit(1);
	}
	
	int listenfd=listen(sockFd,12);
	if(listenfd<0){
		printf("Server:cannot listen\n");
		close(sockFd);
		exit(1);
	}
	while(1){
		int clientAddLen=sizeof(clientAdd);
		int acceptIt=accept(sockFd,( struct sockaddr *)&clientAdd,&clientAddLen);
		if(acceptIt<0){
			printf("SErver error cannot accept the request\n");
			close(sockFd);
			exit(0);
		}
		printf("\nNew Session started\n");	
		while(1){
			int w;
			int r=read(acceptIt,rbuff,sizeof(rbuff));
			if(r<0){
				printf("\nSERVER ERROR:Cannot receive message from client.\n");
			
			}
			else{
				if((strcmp(rbuff,"Bye"))==0){
					close(acceptIt);
					break;
				}
				char wbuff[128];
				char ch;
				int i=0;
				FILE *ptr;
				ptr=fopen(rbuff,"r");
				DIR* dir = opendir(rbuff);
				if (dir!=NULL) {
					w=write(acceptIt,"It is a folder",128);
					if(w<0)
						printf("Cannot sent data sorry\n");

				}else{
				if(ptr!=NULL){
					do{
						ch=getc(ptr);
						wbuff[i++]=ch;
					}while(ch!=EOF);
					
					w=write(acceptIt,wbuff,128);
					if(w<0)
						printf("Cannot sent data sorry\n");
				}
				else{
					w=write(acceptIt,"No such File Exist sorry",128);
					if(w<0)
						printf("Cannot sent data sorry\n");
				}
				}
				
	           	}
	    	}
	    printf("\nClient end the session\n");
     	}
    
	close(sockFd);
	exit(1);
	
}



