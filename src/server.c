#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

#include "tcp.h"
#include "sizes.h"


// Driver function
int main(int argc, char *argv[]) {
    int socket, connfd;
    int port;

    if (argc != 2) {
        printf("arguments: %s <port>\n", argv[0]);
        return 1;
    }

    port = atoi(argv[1]);

    // ############################################################
    // Listen to connections
    // ############################################################
    // TODO: Handle many connections with threads
    socket = TCP_Server_Open(port);
    connfd = TCP_Accept(socket);

    // ############################################################
    // Chatting between client and server
    // ############################################################
    char input[MAX_INPUT];
    char response[MAX_RESPONSE];
    while(1){
        // Clean buffer variables
        bzero(input, MAX_INPUT);
        bzero(response, MAX_RESPONSE);

        // Read client input
        TCP_Read_String(connfd, input, MAX_INPUT);

        // Response preparation
        // TODO: execute the input as a command then send back the O.S response
        strcat(response, "User says: ");
        strcat(response, input);
        printf("%s\n", response);

        // Send response to client
        TCP_Write_String(connfd, input);

        // TODO: remove in the future
        if (!strcmp(input, "exit")){
            printf("bye!\n");
            break;
        }
    }

    // After chatting close the socket
    close(socket);
}
