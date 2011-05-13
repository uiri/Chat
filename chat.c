/*This file is part of UI Chat.

UI Chat is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

UI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with UI.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define PORT "1337"

int main(int argc, char *argv[]) {
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sock, listensock, optstatus, quit=0;
  
  char *recv_buffer;
  recv_buffer = malloc(256 * sizeof(char));
  char *buffer;
  buffer = malloc(256 * sizeof(char));
  
  printf("First message:");
  fgets(buffer, 250, stdin);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((optstatus = getopt (argc, argv, "c:")) != -1 ) {
    switch (optstatus) {
      case 'c':
        getaddrinfo(argv[2], PORT, &hints, &res);
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        connect(sock, res->ai_addr, res->ai_addrlen);
      default:
        hints.ai_flags = AI_PASSIVE;
        getaddrinfo(NULL, PORT, &hints, &res);

        listensock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        bind(listensock,res->ai_addr,res->ai_addrlen);
        listen(listensock, 10);
        sock = accept(listensock,(struct sockaddr *)&client_addr, &addr_size);
    }
  }

  while (quit != 1) {
    send(sock, buffer, 256, 0);
    recv(sock, recv_buffer, 256, 0);
    printf("%sSay:", recv_buffer);
    fgets(buffer, 250, stdin);
  }
  
  freeaddrinfo(res);
  close(sock); 
  free(recv_buffer);
  free(buffer);
  return 0;
}
