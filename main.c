#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg);

int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_in server, client;
	sock = socket(AF_INET,SOCK_STREAM,0);

	if(sock < 0) {
		fprintf(stderr,"cannot open socket");
		exit(1);
	}

	bzero((char *) &server, sizeof(server));
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}


