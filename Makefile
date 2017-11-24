all: client server

client: client.c
	$(CC) $< -o $@

server: server.c
	$(CC) $< -o $@

