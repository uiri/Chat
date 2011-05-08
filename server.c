#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT "10234"

int main(void) {
  //declarations
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sock, client_sock, quit, recvstat;

  //set up buffers
  char *recv_buffer;
  recv_buffer = malloc(256 * sizeof(char));
  char *buffer;
  buffer = malloc(256 * sizeof(char));
  
  /*give an initial message to send.
    we could potentially turn this into an auth handshake so
    that people are talking to who they think they're talking to */
  printf("At any time you can send a blank message to terminate the program\nFirst message:");
  fgets(buffer, 250, stdin);
  quit = strlen(buffer);

  // blank out hints and fill it out with info about the machine
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, PORT, &hints, &res);
  
  //create the socket, bind to it, listen on it, accept when it is connected to
  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  bind(sock,res->ai_addr,res->ai_addrlen);
  listen(sock, 10);
  client_sock = accept(sock,(struct sockaddr *)&client_addr, &addr_size);
  
  /* until SIGINT, send the message to be sent, receive the message to be received
     print out the received message, and get a new message to send */
  while (quit != 1) {
    send(client_sock, buffer, 256, 0);
    recvstat = recv(client_sock, recv_buffer, 256, 0);
    if (recvstat == 1) quit = 1;
    printf("%sSay:", recv_buffer);
    fgets(buffer, 250, stdin);
    quit = strlen(buffer);
  }
  
  //free some stuff up so it closes neatly...
  freeaddrinfo(res);
  close(sock); 
  free(recv_buffer);
  free(buffer);
  return 0;
}
