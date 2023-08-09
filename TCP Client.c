#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>	

struct sockaddr_in serv_addr;

int skfd,r,w;


unsigned short serv_port=5055;

char serv_ip[]="172.18.177.212";

char rbuff[128];
char sbuff[128]="\n Hello Biki\n";

int main(){
	skfd=socket(AF_INET,SOCK_STREAM,0);
	if(skfd<0)
	{
		printf("\nSERVER ERROR:cannot create socket.\n");
		exit(1);
	}
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(serv_port);
	inet_aton(serv_ip,(&serv_addr.sin_addr));
	printf("\t nTCP ECHO SERVER.\n");
	
	
	int CO=connect(skfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));//connect fun call
	if(CO<0){
		printf("\nCLIENT ERROR:Connot connect  to the server  .\n");
		close(skfd);
		exit(1);
	}
	w=write(skfd,sbuff,128);
	if(w<0){
		printf("\nCLIENT ERROR:Cannot send message to the server.\n");
		close(skfd);
		exit(1);
	}
			
	r=read(skfd,rbuff,128);
	if(r<0){
		printf("\nCLIENT ERROR:Cannot recieve message from the server.\n");
		close(skfd);
		exit(1);
	}
	else{
		rbuff[r]='\0';
		printf("\nClient message from echo server %s\n",rbuff);
		close(skfd);
		exit(1);
	}
	close(skfd);
	exit(1);
}
