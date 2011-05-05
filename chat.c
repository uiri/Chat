#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define PORT "1337"

int main(int argc, char *argv[]) {
  // declarations
  int status, sock, acceptsock, clientstatus, connectsock;
  struct addrinfo hints, *res, *p;
  struct sockaddr_storage client_addr;
  char *recv_buffer, *send_buffer;
  recv_buffer = malloc(256 * sizeof(char));
  send_buffer = malloc(256 * sizeof(char));

  // populate hints
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // make sure retrieving address info works
  if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo for server error: %s\n", gai_strerror(status));
    exit(1);
  }
  
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
      perror("server: socket");
      continue;
    }
    
    if (bind(sock, p->ai_addr, p->ai_addrlen) < 0) {
      perror("server: bind");
      continue;
    }
    
    break;
  }

  listen(sock, 10);

   if ((optstatus = getopt (argc, argv, "c:")) != -1) {
        switch (optstatus) {
          case 'c':
            if ((clientstatus = getaddrinfo(argv[1], "1337", &hints, &servinfo)) != 0) {
              fprintf(stderr, "getaddrinfo for client error: %s\n", gai_strerror(clientstatus));
              exit(1);
            }
            for(p = servinfo; p != NULL; p = p->ai_next) {
              if ((connectsock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
                perror("client: socket");
                continue;
              }
              
              if (connect(connectsock, p->ai_addr, p->ai_addrlen) < 0) {
                perror ("client: connect");
                continue;
              }
              
              break;
            }
            send();
        }
      }

  acceptsock = accept(sock,(struct sockaddr *)&client_addr, &addr_size);
  
  freeaddrinfo(servinfo);
  return 0;
}
