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
	int sock, client_sock, quit;
	char first;

	char *buffer;
	buffer = malloc(256 * sizeof(char));
	char *recv_buffer;
	recv_buffer = malloc(256 * sizeof(char));

	scanf("%s\0", buffer);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo("127.0.0.1", PORT, &hints, &res);
	
	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	connect(sock, res->ai_addr, res->ai_addrlen);

	first = *buffer;
	quit = (int)first;
	
	while (quit != 1) {
	  printf("send 1 to quit");
	  send(sock, buffer, 256, 0);
	  scanf("%s\0", buffer);
	  first = *buffer;
	  quit = (int)first;
	  recv(sock, recv_buffer, 256, 0);
	  //printf("Recieved echo: %s\n",recv_buffer);
	}

	freeaddrinfo(res);
	close(sock);
	free(buffer);
	free(recv_buffer);
	return 0;
}
