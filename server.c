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
  
  char *recv_buffer;
  recv_buffer = malloc(256 * sizeof(char));
  char *buffer;
  buffer = malloc(256 * sizeof(char));
  
  printf("First message:");
  fgets(buffer, 250, stdin);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  getaddrinfo("127.0.0.1", PORT, &hints, &res);
  
  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  bind(sock,res->ai_addr,res->ai_addrlen);
  listen(sock, 10);
  client_sock = accept(sock,(struct sockaddr *)&client_addr, &addr_size);
  
  while (quit != 1) {
    send(client_sock, buffer, 256, 0);
    recv(client_sock, recv_buffer, 256, 0);
    printf("%sSay:", recv_buffer);
    fgets(buffer, 250, stdin);
  }
  
  freeaddrinfo(res);
  close(sock); 
  free(recv_buffer);
  free(buffer);
  return 0;
}
