#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
  //declarations
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sock, client_sock, listencount;
  size_t mematch, partmatch;
  mematch = (3 * sizeof(char));
  partmatch = (5 * sizeof(char));

  //check for an argument
  if (argc != 3) {
    fprintf(stderr, "Error: Wrong number of arguments. Usage: ./client $IP_ADDRESS $PORT");
    exit(1);
  }

  //set up buffers
  char *buffer;
  buffer = malloc(256 * sizeof(char));
  char *recv_buffer;
  recv_buffer = malloc(256 * sizeof(char));
  char *name;
  name = malloc(32 * sizeof(char));
  char *recv_name;
  recv_name = malloc(32 * sizeof(char));
  char *mebuffer;
  mebuffer = malloc(256 * sizeof(char));

  /* get an initial message to send
     see the comment at this bit in server.c 
     for an idea of what to do with it later */
  printf("Send /quit to terminate the program but give a name first.\nName: ");
  gets(name);
  printf("%s: ", name);
  fgets(buffer, 250, stdin);

  // blank out hints and fill it out with info about the machine
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  getaddrinfo(argv[1], argv[2], &hints, &res);
  
  // create and connect to a socket
  client_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  connect(client_sock, res->ai_addr, res->ai_addrlen);
  
  send(client_sock, name, 32, 0);
  recv(client_sock, recv_name, 32, 0);
  printf("Now talking to %s\n", recv_name);

  while ((strcmp(buffer, "/quit\n")) != 0) {
    send(client_sock, buffer, 256, 0);
    recv(client_sock, recv_buffer, 256, 0);
    if ((strcmp(recv_buffer, "/quit\n")) == 0) 
      break;
    if ((strncmp(recv_buffer, "/me", mematch)) == 0) {
      mebuffer = strtok(recv_buffer, " ");
      mebuffer = strtok(NULL, "");
      printf("*%s %s%s: ", recv_name, mebuffer, name);
    } else if ((strncmp(recv_buffer, "/part", partmatch)) == 0) {
      close(client_sock);
      client_sock = accept(sock,(struct sockaddr *)&client_addr, &addr_size);
    } else if ((strncmp(recv_buffer, "/join", partmatch)) == 0) {
      close(client_sock);
    } else {
      printf("%s: %s%s: ", recv_name, recv_buffer, name);
    }
    fgets(buffer, 250, stdin);
  }

  if ((strcmp(recv_buffer, "/quit\n")) == 0) {
    printf("Other side quit. Hopefully they had the common courtesy to say good bye first. The very last message was not relayed.\n");
  } else {
    send(client_sock, buffer, 256, 0);
  }

  //free some stuff up so it closes neatly...
  freeaddrinfo(res);
  close(sock);
  free(buffer);
  free(recv_buffer);
  return 0;
}
