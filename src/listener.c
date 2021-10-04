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
dhcpNetworkListener (char *address, int port,
                     dhcpNetworkPktInfo_t (*callbackGetOfferDependencies) (pktDhcpPacket_t
                         *discovery),
                     dhcpNetworkPktInfo_t (*callbackGetAckDependencies) (pktDhcpPacket_t *request))
{
  /* TODO port validation */

  /* TODO address validation */

  /* init */
  int dhcpSocket;

  struct sockaddr_in dhcpServerAddress;

  int retval;

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

      do
        {
          fdReturnedValue = recvfrom (dhcpSocket, requestPkt, DHCP_PACKET_MAX_LEN, 0,
                                      (struct sockaddr *)&dhcpClientAddress, &dhcpClientAddressLen);
        }
      while (pktGetDhcpMessageType (requestPkt) != DHCPDISCOVER);

      if (fork() == 0)
        {
          struct in_addr *requestedIpAddress;

          /* TODO Check requested ip address with ping */

          packetInfo = callbackGetOfferDependencies (requestPkt);

          pktGenOffer (requestPkt, replayPkt, packetInfo.fields, packetInfo.options);

          dhcpClientAddress.sin_addr.s_addr = INADDR_BROADCAST;

          sendto (dhcpSocket, replayPkt, DHCP_PACKET_MAX_LEN, 0,
                  (struct sockaddr *)&dhcpClientAddress, dhcpClientAddressLen);

          do
            {
              fdReturnedValue = recvfrom (dhcpSocket, requestPkt, DHCP_PACKET_MAX_LEN, 0,
                                          (struct sockaddr *)&dhcpClientAddress, &dhcpClientAddressLen);
            }
          while (pktGetDhcpMessageType (requestPkt) != DHCPREQUEST);

          packetInfo = callbackGetAckDependencies (requestPkt);

          pktGenAck (requestPkt, replayPkt, packetInfo.fields, packetInfo.options);

          dhcpClientAddress.sin_addr.s_addr = INADDR_BROADCAST;

          sendto (dhcpSocket, replayPkt, DHCP_PACKET_MAX_LEN, 0,
                  (struct sockaddr *)&dhcpClientAddress, sizeof (dhcpClientAddress));

          /* TODO checking arp for dhcp starvation preventation */
        }
    }
}