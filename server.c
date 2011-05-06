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

	char *recv_buffer;
	recv_buffer = malloc(256 * sizeof(char));
	char *buffer;
	buffer = malloc(256 * sizeof(char));

	scanf("%s\0", buffer);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	getaddrinfo("127.0.0.1", PORT, &hints, &res);
	
	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	bind(sock,res->ai_addr,res->ai_addrlen);
	listen(sock, 10);
	
	client_sock = accept(sock,(struct sockaddr *)&client_addr, &addr_size);
	
	first = *buffer;
	quit = (int)first;

	while (quit != 1) {
	  printf("send 1 to quit");
	  recv(client_sock, recv_buffer, 256, 0);
	  //printf("Recieved: %s\nEchoing to client\n",recv_buffer);
	  send(client_sock, buffer, 256, 0);
	  scanf("%s\0", buffer);
	  first = *buffer;
	  quit = (int)first;
	}

	freeaddrinfo(res);
	close(sock); 
	free(recv_buffer);
	free(buffer);
	return 0;
}
