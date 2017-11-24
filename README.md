# UDP Benchmark for MILA project

This benchmark is composed of three files:

- `client.c` that sends sub-images
- `server.c` that receive sub.images
- `common.h` to configure the server port

To build the project simply type:

```
make
```

Then run the server first on the first server, then the client on another one

```
$ ./client
Build Data...
Configure socket...
Send UDP data...
Time to send 1000 subimages: 2.116311[s]
Finished...
```

```
$ ./server
Configure server...
Bind socket...
Listening...
Total bytes read 33166000
Time to receive 1442000 subimages: 0.169864[s]
Estimated bandwidth 195.250 Mbits/s
Finished...
```
