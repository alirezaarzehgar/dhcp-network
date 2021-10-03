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

  int broadcastEnable = 1;

  dhcpServerAddress.sin_family = AF_INET;

  dhcpServerAddress.sin_port = htons (port);

  dhcpServerAddress.sin_addr.s_addr = inet_addr (address);

  retval = bind (dhcpSocket, (struct sockaddr *)&dhcpServerAddress,
                 sizeof (dhcpServerAddress));

  if (retval == -1)
    return EXIT_FAILURE;

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
          printf ("Hi discovery!\n");

          /* TODO Check requested ip address with ping */

          packetInfo = callbackGetOfferDependencies (requestPkt);

          pktGenOffer (requestPkt, replayPkt, packetInfo.fields, packetInfo.options);

          sendto (dhcpSocket, replayPkt, DHCP_PACKET_MAX_LEN, 0,
                  (struct sockaddr *)&dhcpClientAddress, dhcpClientAddressLen);

          printf ("offer sent : (%d)\n", pktGetDhcpMessageType (replayPkt));

          do
            {
              fdReturnedValue = recvfrom (dhcpSocket, requestPkt, DHCP_PACKET_MAX_LEN, 0,
                                          (struct sockaddr *)&dhcpClientAddress, &dhcpClientAddressLen);
            }
          while (pktGetDhcpMessageType (requestPkt) != DHCPREQUEST);

          packetInfo = callbackGetAckDependencies (requestPkt);

          pktGenAck (requestPkt, replayPkt, packetInfo.fields, packetInfo.options);

          printf ("\nmsg type : %d\n", pktGetDhcpMessageType (replayPkt));

          sendto (dhcpSocket, replayPkt, DHCP_PACKET_MAX_LEN, 0,
                  (struct sockaddr *)&dhcpClientAddress, dhcpClientAddressLen);

          /* TODO recive request and compare with discover */

          /* TODO if everything is OK, send ack */

          /* TODO checking arp for dhcp starvation preventation */
        }
    }
}