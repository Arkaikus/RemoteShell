#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

#include "tcp.h"
#include "sizes.h"


int main(int argc, char *argv[]) {
    int sockfd, port;
    char *host;

    if (argc != 3) {
        printf("arguments: %s <host> <port>\n", argv[0]);
        return 1;
    }
    host = argv[1];
    port = atoi(argv[2]);


    // ############################################################
    // STARTS CONNECTION WITH SERVER
    // ############################################################
    printf("Looking to connect to <%s,%d>\n", host, port);
    sockfd = TCP_Open(Get_IP(host), port);

    // ############################################################
    // STARTS CHAT
    // ############################################################
    char input[MAX_INPUT] = "";
    char response[MAX_RESPONSE];

    // Always run until exit
    while(1){
        // reads user input
        printf("> ");
        fgets(input, sizeof(input), stdin);
        printf("You wrote %s\n", input);

        // removes \n from input
        input[strlen(input) - 1] = '\x0';
        // sends user input
        TCP_Write_String(sockfd, input);

        // Read Server Response
        TCP_Read_String(sockfd, response, MAX_RESPONSE);
        printf("Server says: %s\n", response);  // print response
        bzero(response, MAX_RESPONSE); // clean response

        // loop exit
        if (!strcmp(input, "exit")){
            printf("bye!\n");
            break;
        }
    }

    // close the socket
    close(sockfd);
}
