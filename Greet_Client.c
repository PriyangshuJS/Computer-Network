#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

struct sockaddr_in serv_addr;
int skfd, r, w;

unsigned short serv_port = 5107;
char serv_ip[] = "192.168.24.20";
char rbuff[256];  // Increased buffer size to accommodate time message
char sbuff[128];  // You can keep sbuff as 128 characters

int main() {
    // Socket -
    skfd = socket(AF_INET, SOCK_STREAM, 0);
    if (skfd < 0) {
        printf("\nCLIENT ERROR: Cannot create socket.\n");
        close(skfd);
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    inet_aton(serv_ip, (&serv_addr.sin_addr));
    printf("\nTCP Time Client\n");  // Updated the client description

    // Connect -
    int rd1 = connect(skfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (rd1 < 0) {
        printf("\nCLIENT: Connected to the server.\n");
    }

    while (1) {
        
        if (w < 0) {
            printf("\nCLIENT ERROR: Cannot send the request to the server.\n");
            close(skfd);
            exit(1);
        }

        r = read(skfd, rbuff, sizeof(rbuff));
        if (r < 0) {
            printf("\nCLIENT ERROR: Cannot receive the server response.\n");
        } else {
            rbuff[r] = '\0';
            printf("\nServer Time:\n%s\n", rbuff);
        }
		printf("Type 'STOP' to quit: ");
        scanf("%s", sbuff);

        w = write(skfd, sbuff, strlen(sbuff));

        if (strcmp(sbuff, "STOP") == 0) {
            printf("\nClient Stopped\n");
            close(skfd);
            exit(0);
        }
    }
    close(skfd);
    exit(1);
}
