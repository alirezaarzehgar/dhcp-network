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
                     dhcpNetworkPktInfo_t (*callbackGetReplyDependencies) (pktDhcpPacket_t
                         *discovery),
                     char * (*callbackLeaseOperation) (pktDhcpPacket_t *ack))
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

      pktDhcpPacket_t *replyPkt = (pktDhcpPacket_t *)calloc (sizeof (
                                     pktDhcpPacket_t),
                                   sizeof (pktDhcpPacket_t));

      /* recive discovery */

      dhcpNetworkReciveDiscoveryPkt (dhcpSocket, requestPkt, &dhcpClientAddress,
                                     &dhcpClientAddressLen);

      if (fork() == 0)
        {
          char *errorMsg;
          struct in_addr *requestedIpAddress;

          /* TODO Check requested ip address with ping */

          packetInfo = callbackGetReplyDependencies (requestPkt);

          pktGenOffer (requestPkt, replyPkt, packetInfo.fields, packetInfo.options);

          dhcpNetworkSendBootReplyPkt (dhcpSocket, replyPkt, &dhcpClientAddress,
                                        dhcpClientAddressLen);

          dhcpNetworkReciveRequestPkt (dhcpSocket, requestPkt, replyPkt,
                                       &dhcpClientAddress, &dhcpClientAddressLen);

          packetInfo = callbackGetReplyDependencies (requestPkt);

          pktGenAck (requestPkt, replyPkt, packetInfo.fields, packetInfo.options);


          /* TODO checking arp for dhcp starvation preventation */
          
          if ((errorMsg = callbackLeaseOperation (requestPkt)) == NULL)
            {
              dhcpNetworkSendBootReplyPkt (dhcpSocket, replyPkt, &dhcpClientAddress,
                                            dhcpClientAddressLen);
            }
          else
            {
              pktGenCallback_t options[] =
              {
                {.func = (pktGenCallbackFunc_t)pktGenOptDhcpServerIdentifier, .param = address},
                {.func = (pktGenCallbackFunc_t)pktGenOptMessage, .param = errorMsg},
              };

              pktGenNak (requestPkt, replyPkt, NULL, options);

              dhcpNetworkSendBootReplyPkt (dhcpSocket, replyPkt, &dhcpClientAddress,
                                            dhcpClientAddressLen);
            }

          bzero (reqBuf, DHCP_PACKET_MAX_LEN);

          bzero (replyPkt, DHCP_PACKET_MAX_LEN);

          free (replyPkt);

          replyPkt = NULL;

          exit (EXIT_SUCCESS);
        }
    }
}