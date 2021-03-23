#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "tcp.h"
#include "sizes.h"

struct shell_t{
    int shell_id;
    int socket;
};

int shell_status[MAX_THREADS]={0};

/*
 * This function receives an input command and saves the output in the response variable
 * */
void command(char *input, char *response, int MAX_BUFFER) {
    char buffer[MAX_BUFFER];
    FILE *pipe = popen(input, "r");
    if (!pipe) {
        strcat(response, "Server failed to execute command");
    }

    while (!feof(pipe)) {
        if(fgets(buffer, MAX_BUFFER, pipe) != NULL)
            strcat(response,buffer);
    }

    pclose(pipe);
}

void *open_shell(void * arg){
    struct shell_t *shell = ((struct shell_t*)arg);
    //############################################################
    //Chatting between client and server thread instance
    //############################################################
    char input[MAX_INPUT];
    char response[MAX_RESPONSE];
    while(1){
        // Clean buffer variables
        bzero(input, MAX_INPUT);
        bzero(response, MAX_RESPONSE);

        // Read client input
        TCP_Read_String(shell->socket, input, MAX_INPUT);

        // validate user input
        if (!strcmp(input, "exit") || !strlen(input)){
            printf("Shell %d: bye!\n", shell->shell_id);
            TCP_Write_String(shell->socket, "");
            shell_status[shell->shell_id]=2;
            free(shell);
            break;
        }

        // Response preparation
        printf("Shell %d: %s\n",shell->shell_id, input);
        // Input execution
        command(input, response, MAX_RESPONSE);
        // Mirror response to client
        TCP_Write_String(shell->socket, response);
    }
    return NULL;
}

int available_shell(pthread_t *pool){
    printf("################################################\n");
    // check max number of connections
    for(int i=0; i < MAX_THREADS; i++){
        // change status of finished to available
        if(shell_status[i]==2) {
            pthread_join(pool[i], NULL);
            shell_status[i] = 0;
        }
        // selects available shell
        if(shell_status[i]==0) return i;
    }
    return -1;
};

int main(int argc, char *argv[]) {
    int socket, port, connfd;
    if (argc != 2) {
        printf("arguments: %s <port>\n", argv[0]);
        return 1;
    }

    port = atoi(argv[1]);
    // ############################################################
    // Listen to connections
    // ############################################################
    socket = TCP_Server_Open(port);
    pthread_t threads[MAX_THREADS];

    while(1){
        int shell_id = available_shell(threads);
        printf("Shell %d is available\n", shell_id);
        printf("################################################\n");
        if(shell_id == -1) {
            printf(".");
            continue;
        }
        connfd = TCP_Accept(socket);
        struct shell_t *args = (struct shell_t *)malloc(sizeof(struct shell_t));
        args->shell_id = shell_id;
        args->socket = connfd;
        shell_status[shell_id]=1;
        pthread_create(&threads[shell_id], NULL, open_shell, (void*)args);
    }

    // After chatting close the socket
    close(socket);
}
