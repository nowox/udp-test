/**
 * UDP Server.
 */
#include "common.h"

#define TIMEOUT (5)

char buffer[BUFFER_SIZE];

int main()
{
    struct timespec start, end;
	int32_t frames = 0;
    size_t total = 0;

    printf("Configure server...\n");
	fd_set si_other;
	fd_set original_stdin;
	struct timeval timeout = {.tv_sec = TIMEOUT, .tv_usec = 0};
    struct sockaddr_in si_me;

    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == -1)
	{
		printf("Error in socket(): %d\n", errno);
		return -1;
	}

//	fcntl(s, F_SETFL, fcntl(s, F_GETFL) | O_NONBLOCK);

	FD_ZERO(&si_other);
	FD_SET(s, &si_other);

	bzero(&(si_me.sin_zero),8);
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = INADDR_ANY;

    printf("Bind socket...\n");
	if (bind(s,(struct sockaddr *)&si_me, sizeof(si_me)) == -1)
	{
		printf("Error in bind(): %d\n", errno);
		return -1;
	}

    printf("Listening...\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	do
	{
        int receive = select(s + 1, &si_other, NULL, NULL, &timeout);

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
            total += recv(s, &buffer[total], sizeof(buffer), 0);;
            frames++;
        }
    } while(frames <= SUBIMAGES * FRAMES_PER_SUBIMAGES);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 +
                        (end.tv_nsec - start.tv_nsec) / 1000;

	printf("Total bytes read %ld\n", total);
    printf("Time to receive %ld subimages: %f[s]\n", total / FRAMES_PER_SUBIMAGES, delta_us / 1e6f);
    printf("Finished...\n");
	close(s);
	return 0;
}
