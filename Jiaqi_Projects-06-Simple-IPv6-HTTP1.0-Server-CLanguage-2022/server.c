// A simple server in the internet domain using TCP
// The port number is passed as an argument
// To compile: gcc server.c -o server
// Reference: Beej's networking guide, man pages
#define _POSIX_C_SOURCE 200112L
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "utility.h"

void* operate(void* data);

int main(int argc, char** argv) {
	int sockfd, newsockfd, re, s;
	struct addrinfo hints, *res;
	struct sockaddr_storage client_addr;
	socklen_t client_addr_size;

	if (argc < 4) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(EXIT_FAILURE);
	}
	//if not ipv4 or ipv6
	if ((atoi(argv[1]) != 4) && (atoi(argv[1]) != 6)) {
		exit(EXIT_FAILURE);
	}

	// Create address we're going to listen on (with given port number)
	memset(&hints, 0, sizeof hints);
	if (atoi(argv[1]) == 4) {    // IPv4
		hints.ai_family = AF_INET;  
	} else if (atoi(argv[1]) == 6) {
		hints.ai_family = AF_INET6; 
	}

	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE;     // for bind, listen, accept
	// node (NULL means any interface), service (port), hints, res
	s = getaddrinfo(NULL, argv[2], &hints, &res);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	// Create socket
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Reuse port if possible
	re = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &re, sizeof(int)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	// Bind address to the socket
	if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(res);

	// Listen on socket - means we're ready to accept connections,
	// incoming connection requests will be queued, man 3 listen
	if (listen(sockfd, 5) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// Accept a connection - blocks until a connection is ready to be accepted
	// Get back a new file descriptor to communicate on
	pthread_t tid;
	while(1) {
		client_addr_size = sizeof client_addr;
		newsockfd =
			accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);
		if (newsockfd < 0) {
			close(newsockfd);
		}

		// multiplexing, pthread from workshop 3
		data_t* data = malloc(sizeof(data_t));
		char* absolute_path = argv[3];
		data->absolute_path = absolute_path;
		data->newsockfd = newsockfd;
		if(pthread_create(&tid, NULL, operate, (void*)data) < 0) {
			close(newsockfd);
		}
	}
	close(sockfd);
	return 0;
}


void* operate(void* data) {
	int n, buffer_n = 0;
	char pkt[EACH_READ];
	char buffer[MAXCHAR];
	buffer[0] = '\0';

	// open the data stored in the structure
	data_t* all_data = (data_t*) data;
	int newsockfd = all_data->newsockfd;
	char* absolute_path = all_data->absolute_path;

	// read 1024 bytes each time
	while((n = read(newsockfd, pkt, EACH_READ)) > 0) {

		pkt[n] = '\0';
		
		// store the read data into buffer
		strcat(buffer, pkt);
		buffer_n += n;

		// if read till the end, break
		if (pkt[n-1] == '\n'){
			break;
		}
	}
	
	if (n < 0) {
		close(newsockfd);
	}

	buffer[buffer_n] = '\0';

	if (n>4){
		response(buffer, newsockfd, absolute_path);
	}
	close(newsockfd);
	return NULL;
}


