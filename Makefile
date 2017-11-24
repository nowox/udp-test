all: client server

client: client.c common.h
	$(CC) $< -o $@

server: server.c common.h
	$(CC) $< -o $@

