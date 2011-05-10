#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  //declarations
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sock, client_sock, listencount, backlog, chatval, clientcount;
  char ip[17], port[7];

  char *recv_buffer;
  recv_buffer = malloc(256 * sizeof(char));
  char *buffer;
  buffer = malloc(256 * sizeof(char));
  char *name;
  name = malloc(32 * sizeof(char));
  char *recv_name;
  recv_name = malloc(32 * sizeof(char));

  if (argc < 2) {
    fprintf(stderr, "Too few arguments. Usage: ./main $PORT or ./main -c $IP_ADDRESS $PORT\n");
    exit(1);
  } else if (argc == 3) {
    fprintf(stderr, "Bad number of arguments. Usage: ./main $PORT or ./main -c $IP_ADDRESS $PORT\n");
    exit(1);
  } else if (argc > 4) {
    fprintf(stderr, "Too many arguments. Usage: ./main $PORT or ./main -c $IP_ADDRESS $PORT\n");
    exit(1);
  } else {
    input(name, buffer);
  }

  if ((strcmp(argv[1], "-c")) == 0) {
    clientcount = 1;
  } else {
    clientcount = 0;
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if (clientcount == 1) {
    getaddrinfo(argv[2], argv[3], &hints, &res);
  } else {
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, argv[1], &hints, &res);
  }

  if (clientcount == 1) {
    client_sock = client(res->ai_family, res->ai_socktype, res->ai_protocol, res->ai_addr, res->ai_addrlen);
    listencount = 0;
  } else {
    client_sock = server(sock, res->ai_family, res->ai_socktype, res->ai_protocol, res->ai_addr, res->ai_addrlen, backlog, client_addr, addr_size);
    listencount = 1;
  }

  chatval = chat(client_sock, name, recv_name, buffer, recv_buffer);

  while (chatval != 0) {
    close(client_sock);
    if (chatval == 1) {
      printf("%s", name);
      fgets(buffer, 250, stdin);
      if (listencount == 0) {
        client_sock = server(sock, res->ai_family, res->ai_socktype, res->ai_protocol, res->ai_addr, res->ai_addrlen, backlog, client_addr, addr_size);
        listencount = 1;
      } else {
        client_sock = accept(sock, (struct sockaddr *)&client_addr, &addr_size);
      }
    } else if (chatval == 2) {
      printf("IP ADDRESS:");
      gets(ip);
      printf("PORT:");
      gets(port);
      getaddrinfo(ip, port, &hints, &res);
      client_sock = client(res->ai_family, res->ai_socktype, res->ai_protocol, res->ai_addr, res->ai_addrlen);
    } else {
      printf("Something has gone horribly wrong\n");
    }
    chatval = chat(client_sock, name, recv_name, buffer, recv_buffer);
  }

  if ((strcmp(recv_buffer, "/quit\n")) == 0) {
    printf("Other side quit. Hopefully they had the common courtesy to say good bye first. The very last message was not relayed.\n");
  } else {
    send(client_sock, buffer, 256, 0);
  }

  freeaddrinfo(res);
  close(client_sock);
  if (listencount == 1)
    close(sock);
  return 0;
}

int chat(int sock, char *name, char *recv_name, char *buffer, char *recv_buffer) {
  size_t mematch, partmatch;
  mematch = (3 * sizeof(char));
  partmatch = (5 * sizeof(char));

  char *mebuffer;
  mebuffer = malloc(256 * sizeof(char));

  send(sock, name, 32, 0);
  recv(sock, recv_name, 32, 0);
  printf("Now talking to %s\n", recv_name);
  
  while ((strcmp(buffer, "/quit\n")) != 0) {
    send(sock, buffer, 256, 0);
    recv(sock, recv_buffer, 256, 0);
    if ((strcmp(recv_buffer, "/quit\n")) == 0) {
      return 0;
      break;
    }
    if ((strncmp(recv_buffer, "/me", mematch)) == 0) {
      mebuffer = strtok(recv_buffer, " ");
      mebuffer = strtok(NULL, "");
      printf("*%s %s%s: ", recv_name, mebuffer, name);
    } 
    else if ((strncmp(buffer, "/part", partmatch)) == 0) {
      return 1;
      break;
    } 
    else if ((strncmp(buffer, "/join", partmatch)) == 0) {
      return 2;
      break;
    } 
    else {
      printf("%s: %s%s: ", recv_name, recv_buffer, name);
    }
    fgets(buffer, 250, stdin);
  }
}

int input(char *name, char *buffer) {
  printf("Send /quit to terminate the program but give a name first.\nName: ");
  gets(name);
  printf("%s: ", name);
  fgets(buffer, 250, stdin);  
}

int client(int domain, int type, int protocol, struct sockaddr *serv_addr, int addrlen) {
  int returnvalue;
  returnvalue = socket(domain, type, protocol);
  connect(returnvalue, serv_addr, addrlen);
  return returnvalue;
}

int server(int sock, int domain, int type, int protocol, struct sockaddr *serv_addr, int addrlen, int backlog, struct sockaddr_storage client_addr, socklen_t addr_size) {
  int returnvalue;
  sock = socket(domain, type, protocol);
  bind(sock, serv_addr, addrlen);
  listen(sock, backlog);
  addr_size = sizeof client_addr;
  returnvalue = accept(sock, (struct sockaddr *)&client_addr, &addr_size);
  return returnvalue;
}
