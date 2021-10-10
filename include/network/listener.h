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

static inline void
dhcpNetworkReciveBootRequestPkt (int dhcpSocket, pktDhcpPacket_t *requestPkt,
                                 pktDhcpPacket_t *previousReplyPkt, struct sockaddr_in *dhcpClientAddress,
                                 socklen_t *dhcpClientAddressLen, int pktType)
{
  int fdReturnedValue = 0;

  do
    {
      fdReturnedValue = recvfrom (dhcpSocket, requestPkt, DHCP_PACKET_MAX_LEN, 0,
                                  (struct sockaddr *)dhcpClientAddress, dhcpClientAddressLen);

      if (fdReturnedValue >= 0 && pktType == DHCPREQUEST
          && previousReplyPkt != NULL && requestPkt->xid == previousReplyPkt->xid)
        break;
      else
        continue;
    }
  while (pktGetDhcpMessageType (requestPkt) != pktType);
}

static inline void
dhcpNetworkReciveDiscoveryPkt (int dhcpSocket, pktDhcpPacket_t *requestPkt,
                               struct sockaddr_in *dhcpClientAddress, socklen_t *dhcpClientAddressLen)
{
  dhcpNetworkReciveBootRequestPkt (dhcpSocket, requestPkt, NULL,
                                   dhcpClientAddress, dhcpClientAddressLen, DHCPDISCOVER);
}

static inline void
dhcpNetworkReciveRequestPkt (int dhcpSocket, pktDhcpPacket_t *requestPkt,
                             pktDhcpPacket_t *previousReplyPkt, struct sockaddr_in *dhcpClientAddress,
                             socklen_t *dhcpClientAddressLen)
{
  dhcpNetworkReciveBootRequestPkt (dhcpSocket, requestPkt, previousReplyPkt,
                                   dhcpClientAddress, dhcpClientAddressLen, DHCPREQUEST);
}

static inline void
dhcpNetworkSendBootReplyPkt (int dhcpSocket, pktDhcpPacket_t *replyPkt,
                             struct sockaddr_in *dhcpClientAddress, socklen_t dhcpClientAddressLen)
{
  dhcpClientAddress->sin_addr.s_addr = INADDR_BROADCAST;

  sendto (dhcpSocket, replyPkt, DHCP_PACKET_MAX_LEN, 0,
          (struct sockaddr *)dhcpClientAddress, dhcpClientAddressLen);
}

int dhcpNetworkSocketInit (int port);

int dhcpNetworkListener (char *address, int port,
                         dhcpNetworkPktInfo_t (*callbackGetReplyDependencies) (pktDhcpPacket_t
                             *discovery),
                         char * (*callbackLeaseOperation) (pktDhcpPacket_t *ack));

#endif