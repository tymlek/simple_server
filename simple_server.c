#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "hacking.h"

#define BUFF_SIZE 1024
#define PORT 7890		// The port user will be connecting to

sig_atomic_t volatile isrunning = 1;

void sig_handler(int signum);

int main(void) {
	int sockfd = 0;		// Listen on sock_fd
	int new_sockfd = 0;	// New connection on new_fd
	struct sockaddr_in host_addr;	// My address information
	struct sockaddr_in client_addr;
	socklen_t sin_size;
	int recv_length = 1;
	int yes = 1;
	char buffer[BUFF_SIZE];
	
	signal(SIGINT, &sig_handler);

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		fatal("in socket");
	} // end if

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int)) == -1) {
		fatal("setting socket option SO_REUSEADDR");
	} //end if

	host_addr.sin_family = AF_INET;		// Host byte order
	host_addr.sin_port = htons(PORT);	// Short, network byte order
	host_addr.sin_addr.s_addr = 0;		// Automatically fill with my IP
	memset(&(host_addr.sin_zero), '\0', 8);	// Zero the rest of the struct

	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof (struct sockaddr)) == -1) {
		fatal("binding to socket");
	} // end if

	if (listen(sockfd, 5) == -1) {
		fatal("listening on socket");
	} // end if

	while (isrunning) {	// Accept loop
		sin_size = sizeof (struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if (new_sockfd == -1) {
			fatal("accepting connection");
		} // end if

		printf("server: got connection from %s port %d\n", 
				inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		send(new_sockfd, "Hello, wold!\n", 13, 0);
		recv_length = recv(new_sockfd, &buffer, BUFF_SIZE, 0);

		while (recv_length > 0) {
			printf("RECV: %d bytes\n", recv_length);
			dump(buffer, recv_length);
			recv_length = recv(new_sockfd, &buffer, BUFF_SIZE, 0);
		} // end while

		close(new_sockfd);
	} // end while

	close(sockfd);
	return (EXIT_SUCCESS);
} // end main

void sig_handler(int signum)
{
	if (signum == SIGINT) {
		//close(sockfd);
		//close(new_sockfd);
		isrunning = 0;
		signal(SIGINT, SIG_DFL);
	} // end if
} // end sig_handler
