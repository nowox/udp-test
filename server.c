/**
 * UDP Server.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "common.h"

#define TIMEOUT (5)

char buffer[BUFFER_SIZE];

int main()
{
    struct timespec start, end;
	int32_t frames = 0;
    size_t total = 0;

    printf("Configure server...\n");
	fd_set original_socket;
	fd_set original_stdin;
	fd_set readfds;
	struct timeval timeout = {.tv_sec = TIMEOUT, .tv_usec = 0};
    struct sockaddr_in server_address;

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_fd == -1)
	{
		printf("Error in socket(): %d\n", errno);
		return -1;
	}

	fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL) | O_NONBLOCK);

	FD_ZERO(&original_socket);
	FD_ZERO(&original_stdin);
	FD_ZERO(&readfds);

	FD_SET(socket_fd, &original_socket);
	FD_SET(socket_fd, &readfds);
	FD_SET(0,&original_stdin);

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_address.sin_zero),8);

    printf("Bind socket...\n");
	if (bind(socket_fd,(struct sockaddr *)&server_address, sizeof(struct sockaddr)) == -1)
	{
		printf("Error in bind(): %d\n", errno);
		return -1;
	}

    printf("Listening...\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	do
	{
		readfds = original_socket;
        int receive = select(socket_fd + 1, &readfds, NULL, NULL, &timeout);

        if (receive == 0)
        {
            fprintf(stderr, "Timeout, lost data.\n");
            return -1;
        }
        else if (receive < 0)
		{
            fprintf(stderr, "Error in select(): %d\n", errno);
			return -1;
		}
		else
		{
            total += recv(socket_fd, &buffer[total], sizeof(buffer), 0);;
            frames++;
        }
    } while(frames <= SUBIMAGES * FRAMES_PER_SUBIMAGES);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    printf("Total sub-images %d\n", SUBIMAGES);
	printf("Total frames %d\n", frames);
	printf("Total bytes read %ld\n", total);

    uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 +
                        (end.tv_nsec - start.tv_nsec) / 1000;

    printf("Time to receive %ld subimages: %f[s]\n", total / FRAMES_PER_SUBIMAGES, delta_us / 1e6f);
    printf("Finished...\n");
	close (socket_fd);
	return 0;
}
