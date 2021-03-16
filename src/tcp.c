#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "tcp.h"

// Esta funcion es usada para crear un servidor de socket.
// El usuario pasa el numero de puerto donde desea que este servidor
// sea accedido.
int TCP_Server_Open(int port) {
  int sockfd;
  struct sockaddr_in servaddr;

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("socket creation failed...\n");
    exit(0);
  } else
    printf("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));
  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0) {
    printf("socket bind failed...\n");
    exit(0);
  } else
    printf("Socket successfully binded..\n");

  // Now server is ready to listen and verification
  if ((listen(sockfd, 5)) != 0) {
    printf("Listen failed...\n");
    exit(0);
  } else
    printf("Server listening..\n");

  return sockfd;
}

// Esta funcion espera por conexiones de red desde un cliente
// Devuelve un socket que es el canal de comunicacion entre el
// servidor y el cliente que solicita la conexion
int TCP_Accept(int socket) {
  struct sockaddr_in servaddr, cli;
  int len = sizeof(cli);
  int connfd;

  printf("Server accepting connections..\n");
  // Accept the data packet from client and verification
  connfd = accept(socket, (SA *)&cli, &len);
  if (connfd < 0) {
    printf("Server acccept failed...\n");
    exit(0);
  } else
    printf("Server acccept the client...\n");

  return connfd;
}

// Esta es la funcion que usa un cliente para conectarse a un
// servidor.
int TCP_Open(char *host, int port) {
  int sockfd;
  struct sockaddr_in servaddr;

  // socket create and varification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("socket creation failed...\n");
    exit(0);
  } else
    printf("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));
  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(host);
  servaddr.sin_port = htons(port);

  // connect the client socket to server socket
  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0) {
    printf("Connection with the server failed...\n");
    exit(0);
  } else
    printf("Connected to the server..\n");

  return sockfd;
}

int TCP_Close(int sck) { close(sck); }

int TCP_Write_String(int socket, char *string) {
  int len = strlen(string) * sizeof(char);
  int written;
  written = write(socket, string, len);
  if (len != written) {
    printf("Sent %d/%d\n", written, len);
  }
  return written;
}

int TCP_Read_String(int socket, char *string, int maxstring) {
  int _read;
  _read = read(socket, string, maxstring);

  printf("Received %d/%d\n", _read, maxstring);

  return _read;
}

char *Get_IP(char *hostname) {
  struct hostent *host_entry;

  host_entry = gethostbyname(hostname);
  if (host_entry == NULL) {
    printf("host_entry failed..");
    return NULL;
  }
  return inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
}
