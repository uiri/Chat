#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <regex.h>

#define PORT "10234"

int main(int argc, char *argv[]) {
  //declarations
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sock, client_sock, quit;

  //check for an argument
  if (argc != 2) {
    fprintf(stderr, "Error: Wrong number of arguments. Usage: ./client $IP_ADDRESS");
    exit(1);
  }

  //set up buffers
  char *buffer;
  buffer = malloc(256 * sizeof(char));
  char *recv_buffer;
  recv_buffer = malloc(256 * sizeof(char));

  /* get an initial message to send
     see the comment at this bit in server.c 
     for an idea of what to do with it later */
  printf("First message:");
  fgets(buffer, 250, stdin);

  // blank out hints and fill it out with info about the machine
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  getaddrinfo(argv[1], PORT, &hints, &res);
  
  // create and connect to a socket
  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  connect(sock, res->ai_addr, res->ai_addrlen);
  
  /* until SIGINT, send the message to be sent, receive the message to be received
     print out the received message, and get a new message to send */
  while (quit != 1) {
    send(sock, buffer, 256, 0);
    recv(sock, recv_buffer, 256, 0);
    printf("%sSay:", recv_buffer);
    fgets(buffer, 250, stdin);
  }

  //free some stuff up so it closes neatly...
  freeaddrinfo(res);
  close(sock);
  free(buffer);
  free(recv_buffer);
  return 0;
}
