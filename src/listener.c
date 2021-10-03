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
#include "pkt/dhcp.h"

int
dhcpNetworkListener (char *address, int port)
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

  dhcpServerAddress.sin_family = AF_INET;

  dhcpServerAddress.sin_port = htons (port);

  dhcpServerAddress.sin_addr.s_addr = inet_addr (address);

  retval = bind (dhcpSocket, (struct sockaddr *)&dhcpServerAddress,
                 sizeof (struct sockaddr));

  if (retval == -1)
    return EXIT_FAILURE;

  while (1)
    {
      struct sockaddr_in dhcpClientAddress;

      socklen_t dhcpClientAddressLen;

      int fdReturnedValue = 0;

      char buf[DHCP_PACKET_MAX_LEN];

      pktDhcpPacket_t *packet = (pktDhcpPacket_t *)buf;

      /* recive discovery */

      while (fdReturnedValue <= 0)
        {
          fdReturnedValue = recvfrom (dhcpSocket, packet, DHCP_PACKET_MAX_LEN, 0,
                                      (struct sockaddr *)&dhcpClientAddress, &dhcpClientAddressLen);
        }

      /* TODO Check requested ip address with ping */

      /* `suitableIpAddressForLease = callback (discovery);` */

      /* TODO send offer with `suitableIpAddressForLease` informations */

      /* TODO recive request and compare with discover */

      /* TODO if everything is OK, send ack */

      /* TODO checking arp for dhcp starvation preventation */
    }
}