#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int locsock;
  int remsock;
  int local_status;
  int remote_status;
  int optstatus;
  struct addrinfo local, *locinfo;
  struct addrinfo remote, *reminfo;
  
  /* if (argc < 2) {
       printf(stderr, "too few arguments");
     } */

  memset(&local, 0, sizeof local);
  local.ai_family = AF_UNSPEC;
  local.ai_socktype = SOCK_STREAM;
  local.ai_flags = AI_PASSIVE;

  memset(&remote, 0, sizeof remote);
  remote.ai_family = AF_UNSPEC;
  remote.ai_socktype = SOCK_STREAM;

  if ((local_status = getaddrinfo(NULL, "1337", &local, &locinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(local_status));
    exit(1);
  }
  
  locsock = socket(locinfo->ai_family, locinfo->ai_socktype, locinfo->ai_protocol);
  bind(locsock, locinfo->ai_addr, locinfo->ai_addrlen);
  
  if ((optstatus = getopt (argc, argv, "c:")) != -1) {
    switch (optstatus) {
      case 'c':
        remote_status = getaddrinfo(argv[1], "1337", &remote, &reminfo);
        /* remsock = socket(reminfo->ai_family, reminfo->ai_socktype, reminfo->ai_protocol); // this line is segfaulting :(
           printf("%d\n", remsock);  // this line tests the line above */
    }
  }
  
  freeaddrinfo(locinfo);
  freeaddrinfo(reminfo);
  return 0;
}
