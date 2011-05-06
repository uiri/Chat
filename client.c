#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT "10234"

int main(void) {

  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sock, client_sock, quit, recvstat;
  int buffer_size = 250;
  
  char *buffer;
  buffer = malloc(256 * sizeof(char));
  char *recv_buffer;
  recv_buffer = malloc(256 * sizeof(char));
  
  printf("First message:");
  fgets(buffer, 250, stdin);

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  getaddrinfo("127.0.0.1", PORT, &hints, &res);
  
  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  connect(sock, res->ai_addr, res->ai_addrlen);
  
  while (quit != 1) {
    send(sock, buffer, 256, 0);
    recv(sock, recv_buffer, 256, 0);
    printf("%sSay:", recv_buffer);
    fgets(buffer, 250, stdin);
    if (buffer[0] == "/") {
      quit=1;
    }
  }

  freeaddrinfo(res);
  close(sock);
  free(buffer);
  free(recv_buffer);
  return 0;
}
