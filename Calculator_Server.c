#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

struct sockaddr_in serv_addr, cli_addr;

int listenfd, connfd, r, w, cli_addr_len, bin, ltn;

unsigned short serv_port = 5101;
char serv_ip[] = "127.0.0.1";
char op1[128], op2[128];

int res = 0;
char resbuff[128];
char operator[2];
int i = 1;

int main() {
    // Socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        printf("\nSERVER ERROR: Cannot create socket.\n");
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    inet_aton(serv_ip, &serv_addr.sin_addr);
    printf("\nTCP ECHO SERVER. \n");

    // Bind
    bin = bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (bin < 0) {
        printf("\nSERVER ERROR: Cannot bind,\n");
        close(listenfd);
        exit(1);
    }

    // Listen
    ltn = listen(listenfd, 15);
    if (ltn < 0) {
        printf("\nSERVER ERROR: Cannot listen.\n");
        close(listenfd);
        exit(1);
    }
    cli_addr_len = sizeof(cli_addr);
    while (1) {
        connfd = accept(listenfd, (struct sockaddr *) &cli_addr, &cli_addr_len);
        if (connfd < 0) {
            printf("\n SERVER ERROR: Cannot accept client. \n");
            close(listenfd);
            exit(1);
        }
        printf("\n Server: Connection from client is accepted\n", inet_ntoa(cli_addr.sin_addr));

        r = read(connfd, op1, 128);
        if (r < 0) {
            printf("\nSERVER ERROR : Cannot receive messages from client.\n");
            close(connfd);
            exit(1);
        } else {
            printf("\nOperand 1 -- %s", op1);
        }

        r = read(connfd, op2, 128);
        if (r < 0) {
            printf("\nSERVER ERROR : Cannot receive messages from client.\n");
            close(connfd);
            exit(1);
        } else {
            printf("\nOperand 2 -- %s", op2);
        }

        r = read(connfd, operator, 2); // Changed the buffer size to 2
        if (r < 0) {
            printf("\nSERVER ERROR : Cannot receive messages from client.\n");
            close(connfd);
            exit(1);
        } else {
            printf("\nOperator- %s", operator);
        }

        int x = atoi(op1);
        int y = atoi(op2);
        if (y == 0 && operator[0] == '/') { // Changed operator to operator[0]
            strncpy(resbuff, "Can't perform the operation!!", sizeof(resbuff));
            w = write(connfd, resbuff, sizeof(resbuff));
            if (w < 0) {
                printf("\nServer Error: Cannot send message to the client.\n");
                close(connfd);
                exit(1);
            }
        }

        // Operation
        switch (operator[0]) { // Changed operator to operator[0]
            case '+':
                res = x + y;
                break;
            case '-':
                res = x - y;
                break;
            case '*':
                res = x * y;
                break;
            case '/':
                res = x / y;
                break;
            default:
                printf("Operator is not correct!!!");
        }

        // Write
        snprintf(resbuff, sizeof(resbuff), "%d", res); // Changed sprintf to snprintf
        w = write(connfd, resbuff, sizeof(resbuff));
        if (w < 0) {
            printf("\nServer Error: Cannot send message to the client.\n");
            close(connfd);
            exit(1);
        } else {
            printf("The Result of the Operation is %d", res);
        }
    }
}
