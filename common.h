#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>

#define PORT (3000)

#define SUBIMAGES (1000)
#define FRAMES_PER_SUBIMAGES (23)
#define UDP_FRAME (1442)

#define SERVERADDRESS "127.0.0.1" // 138.131.156.36"

#define BUFFER_SIZE (SUBIMAGES * FRAMES_PER_SUBIMAGES * UDP_FRAME)

#endif /* COMMON_H */
