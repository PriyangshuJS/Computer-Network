#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>	

struct sockaddr_in serv_addr,cli_addr;

int listenfd,connfd,r,w,va,cli_addr_len;

unsigned short serv_port=5055;

char serv_ip[]="172.18.177.212";

char buff[128];

int main(){
	//SOCKET()
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd<0)
	{
		printf("\nSERVER ERROR:cannot create socket.\n");
		exit(1);
	}
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(serv_port);//host to network short - htons
	inet_aton(serv_ip,(&serv_addr.sin_addr));//address to network - aton
	printf("\t nTCP ECHO SERVER.\n");
	
	int bi=bind(listenfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));//bind fun call
	if(bi<0){
		printf("\nSERVER ERROR:cannot bind .\n");
		close(listenfd);
		exit(1);
	}
	int li=listen(listenfd,5);//listen info
	if(li<0)
	{
		printf("\nSERVER ERROR:connot listen.\n");
		close(listenfd);
		exit(1);
	}
	cli_addr_len=sizeof(cli_addr);
	for(;;){
		printf("\nSERVER:Listening for clients...press ctrl+c to stop echo server:\n");
		connfd = accept(listenfd,(struct sockaddr *)&cli_addr,&cli_addr_len);
		if(connfd <0){
			printf("\nSERVER ERROR;Cannot accept client connections.\n");
			close(listenfd);
			exit(1);
		}
		printf("\nSERVER:Connection from client %s accepted.\n",inet_ntoa(cli_addr.sin_addr));
		r=read(connfd,buff,128);
		if(r<0){
			printf("\nSERVER ERROR:Cannot receive message from client.\n");
			
		}
		else{
			buff[r]="\n Bik is good";
			w=write(connfd,buff,128);
			if(w<0)
				printf("\nSERVER ERROR:Cannot send message to the client.\n");
				
			else
				printf("\nSERVER:Echoed back %s  to %s .\n",buff,inet_ntoa(cli_addr.sin_addr));
			
	           }
              }
}
