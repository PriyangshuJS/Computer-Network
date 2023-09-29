#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

struct sockaddr_in serv_addr, cli_addr;

int listenfd, connfd, r, w, val, cli_addr_len, bin, ltn;
time_t tm;
int hour; 
char greeting[32]="Good Morning";

unsigned short serv_port = 5107;
char serv_ip[] = "192.168.24.20";
char buff[128];

int main() 
{
    // Socket -
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) 
    {
        printf("\nSERVER ERROR: Cannot create socket.\n");
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    inet_aton(serv_ip, (&serv_addr.sin_addr));
    printf("\nTCP File Server.\n");

    // Bind -
    bin = bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (bin < 0) 
    {
        printf("\nSERVER ERROR: Cannot bind.\n");
        close(listenfd);
        exit(1);
    }

    // Listen -
    ltn = listen(listenfd, 15);
    if (ltn < 0) 
    {
        printf("\nSERVER ERROR: Cannot listen.\n");
        close(listenfd);
        exit(1);
    }
    cli_addr_len = sizeof(cli_addr);
    while (1) 
    {
        connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
        if (connfd < 0) 
        {
            printf("\n SERVER ERROR: Cannot accept client. \n");
            close(listenfd);
            exit(1);
        }

        tm = time(NULL);
        struct tm *local_time = localtime(&tm);
        char *timeStr = ctime(&tm); 
    
    if (strlen(timeStr) >= 13) {
        char hourStr[3]; 
        hourStr[0] = timeStr[11];
        hourStr[1] = timeStr[12];
        hourStr[2] = '\0'; 
        
        hour = atoi(hourStr); 
        greeting[32]="Good Morning";
        if (hour< 12)
            snprintf(greeting, sizeof(greeting), "Good Morning");
        else if(hour >12 && hour <16)
            snprintf(greeting, sizeof(greeting), "Good Afternoon");
		else if(hour >16 && hour <21)
            snprintf(greeting, sizeof(greeting), "Good Evening");
		else
            snprintf(greeting, sizeof(greeting), "Good Night");
    }
        

       
        snprintf(buff, sizeof(buff), "%s,\nClient - %s\nToday's date is - %s\n", greeting, inet_ntoa(cli_addr.sin_addr), ctime(&tm));

        w = write(connfd, buff, strlen(buff));
        if (w < 0) 
        {
            printf("\nSERVER ERROR: Failed to send data to client.\n");
        }

        close(connfd);
    }

    return 0;
}

//
