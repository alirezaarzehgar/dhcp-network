/**
 * @file listener.c
 * @author alirezaarzegar (alirezaarzegar82@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-03
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "network/listener.h"

int
dhcpNetworkSocketInit (int port)
{
  int retval;

  int dhcpSocket;

  struct sockaddr_in dhcpServerAddress;

  dhcpSocket = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (dhcpSocket == -1)
    return EXIT_FAILURE;

  int enable = 1;

  retval = setsockopt (dhcpSocket, SOL_SOCKET, SO_BROADCAST, &enable,
                       sizeof (enable));
  if (retval == -1)
    return EXIT_FAILURE;

  retval = setsockopt (dhcpSocket, SOL_SOCKET, SO_REUSEADDR, &enable,
                       sizeof (enable));

  if (retval == -1)
    return EXIT_FAILURE;

  retval = setsockopt (dhcpSocket, SOL_SOCKET, SO_REUSEPORT, &enable,
                       sizeof (enable));

  if (retval == -1)
    return EXIT_FAILURE;

  dhcpServerAddress.sin_family = AF_INET;

  dhcpServerAddress.sin_port = htons (port);

  dhcpServerAddress.sin_addr.s_addr = INADDR_ANY;

  retval = bind (dhcpSocket, (struct sockaddr *)&dhcpServerAddress,
                 sizeof (dhcpServerAddress));

  if (retval == -1)
    return EXIT_FAILURE;

  return dhcpSocket;
}

int
dhcpNetworkListener (char *address, int port,
                     dhcpNetworkPktInfo_t (*callbackGetOfferDependencies) (pktDhcpPacket_t
                         *discovery),
                     dhcpNetworkPktInfo_t (*callbackGetAckDependencies) (pktDhcpPacket_t *request))
{
  /* TODO port validation */

  /* TODO address validation */

  /* init */
  int dhcpSocket;

  int retval;

  dhcpSocket = dhcpNetworkSocketInit (port);

  if (dhcpSocket == 1)
    return EXIT_FAILURE;

  /* listener */
  while (1)
    {
      struct sockaddr_in dhcpClientAddress;

      socklen_t dhcpClientAddressLen;

      int fdReturnedValue = 0;

      char reqBuf[DHCP_PACKET_MAX_LEN];

      dhcpNetworkPktInfo_t packetInfo;

      pktDhcpPacket_t *requestPkt = (pktDhcpPacket_t *)reqBuf;

      pktDhcpPacket_t *replayPkt = (pktDhcpPacket_t *)calloc (sizeof (
                                     pktDhcpPacket_t),
                                   sizeof (pktDhcpPacket_t));

      /* recive discovery */

      dhcpNetworkReciveDiscoveryPkt (dhcpSocket, requestPkt, &dhcpClientAddress,
                                     &dhcpClientAddressLen);

      if (fork() == 0)
        {
          struct in_addr *requestedIpAddress;

          /* TODO Check requested ip address with ping */

          packetInfo = callbackGetOfferDependencies (requestPkt);

          pktGenOffer (requestPkt, replayPkt, packetInfo.fields, packetInfo.options);

          dhcpNetworkSendBootReplayPkt (dhcpSocket, replayPkt, &dhcpClientAddress,
                                        dhcpClientAddressLen);

          dhcpNetworkReciveRequestPkt (dhcpSocket, requestPkt, replayPkt,
                                       &dhcpClientAddress, &dhcpClientAddressLen);

          packetInfo = callbackGetAckDependencies (requestPkt);

          pktGenAck (requestPkt, replayPkt, packetInfo.fields, packetInfo.options);

          dhcpNetworkSendBootReplayPkt (dhcpSocket, replayPkt, &dhcpClientAddress,
                                        dhcpClientAddressLen);

          /* TODO checking arp for dhcp starvation preventation */
          printf ("Lease!\n");

          bzero (reqBuf, DHCP_PACKET_MAX_LEN);

          bzero (replayPkt, DHCP_PACKET_MAX_LEN);

          free (replayPkt);

          replayPkt = NULL;

          exit (EXIT_SUCCESS);
        }
    }
}