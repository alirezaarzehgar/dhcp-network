#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(NETWORK_LISTENER_H)
#define NETWORK_LISTENER_H

int dhcpNetworkListener (char *address, int port);

#endif