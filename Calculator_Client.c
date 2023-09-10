#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

struct sockaddr_in serv_addr;
int skfd, r, w, con;

unsigned short serv_port = 5101;
char serv_ip[] = "127.0.0.1";
char op1[128], op2[128], res[128];
char operator[2]; // Changed buffer size to 2

char again = 'y';

int main() {
    // Socket
    skfd = socket(AF_INET, SOCK_STREAM, 0);
    if (skfd < 0) {
        printf("\nCLIENT ERROR: Cannot create socket.\n");
        close(skfd);
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    inet_aton(serv_ip, &serv_addr.sin_addr);
    printf("\n TCP ECHO CLIENT \n");

    // Connect
    con = connect(skfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (con < 0) {
        printf("\nCLIENT ERROR: Failed to connect to the server.\n");
        close(skfd);
        exit(1);
    }

    // Calculator
    while (again != 'n') {
        printf("\nEnter the first operand - ");
        fgets(op1, sizeof(op1), stdin);
        printf("Enter the Second operand - ");
        fgets(op2, sizeof(op2), stdin);
        printf("Enter the operator - ");
        fgets(operator, sizeof(operator), stdin);

        w = write(skfd, op1, sizeof(op1));
        if (w < 0) {
            printf("\n CLIENT ERROR: Cannot send message to the echo server.\n");
            close(skfd);
            exit(1);
        }

        w = write(skfd, op2, sizeof(op2));
        if (w < 0) {
            printf("\n CLIENT ERROR: Cannot send message to the echo server.\n");
            close(skfd);
            exit(1);
        }

        w = write(skfd, operator, sizeof(operator));
        if (w < 0) {
            printf("\n CLIENT ERROR: Cannot send message to the echo server.\n");
            close(skfd);
            exit(1);
        } else {
            r = read(skfd, res, sizeof(res));
            if (r < 0) {
                printf("\nCLIENT ERROR: Cannot receive message from server.\n");
            } else {
                puts(res);
                int ans = atoi(res);
                printf("\nCLIENT: Message from echo server: %d\n", ans);
            }
        }

        printf("\nAgain? - y/n\n");
        scanf(" %c", &again); // Use a space before %c to consume the newline character
    }

    close(skfd); // Close the socket
    return 0; // Return 0 to indicate successful program termination
}
