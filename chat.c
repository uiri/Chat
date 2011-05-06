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
  int status, sock, acceptsock, clientstatus, connectsock, optstatus;
  socklen_t addr_size;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage client_addr;
  char *recv_buffer, *send_buffer, *quit="n", *cflag="n";

  recv_buffer = malloc(256 * sizeof(char));
  send_buffer = malloc(256 * sizeof(char));
  quit = malloc(1 * sizeof(char));
  cflag = malloc(1 * sizeof(char));

  // populate hints
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((optstatus = getopt (argc, argv, "c:")) != -1) {
    switch (optstatus) {
      case 'c':
	cflag="y";

	if ((clientstatus = getaddrinfo(argv[2], "1337", &hints, &servinfo)) != 0) {
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
	  
	  while(quit != "y") {
	    scanf("%s\0", send_buffer);
	    send(connectsock, send_buffer, 256, 0);
	    recv(connectsock, recv_buffer, 256, 0);
	    printf("%s\n", recv_buffer);
	    /*	    printf("quit?:");
	    scanf("%d", quit);
	    send(connectsock, quit, 1, 0); */
	  }
	  
	  break;
	}
    }
  }

  if (cflag != "y") {
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
    acceptsock = accept(sock,(struct sockaddr *)&client_addr, &addr_size);

    while (cflag != "y") {
      printf("Send a message:\n");
      scanf("%s\n", send_buffer);
      printf("sending message...");
      send(acceptsock, send_buffer, 256, 0);
      printf("message sent\nreceiving message...");
      recv(acceptsock, recv_buffer, 256, 0);
      printf("message received\n");
      printf("%s\n", recv_buffer);
      recv(acceptsock, cflag, 1, 0);
    }
  }
  
  freeaddrinfo(servinfo);
  free(send_buffer);
  free(recv_buffer);
  free(quit);
  free(cflag);
  return 0;
}
