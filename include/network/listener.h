#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "pkt/analyze.h"
#include "pkt/generate.h"

#if !defined(NETWORK_LISTENER_H)
#define NETWORK_LISTENER_H

typedef struct
{
  pktGenCallback_t fields[16];
  pktGenCallback_t options[225];
} dhcpNetworkPktInfo_t;


int dhcpNetworkListener (char *address, int port,
                         dhcpNetworkPktInfo_t (*callbackGetOfferDependencies) (pktDhcpPacket_t* discovery),
                         dhcpNetworkPktInfo_t (*callbackGetAckDependencies) (pktDhcpPacket_t* request));

#endif