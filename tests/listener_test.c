/**
 * @file listener_test.c
 * @author forghani_m
 * @brief  test for network_listener - inline functions
 * @version 0.1.0
 * @date 12 Oct 2021
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <sys/socket.h>
#include <stdio.h>

#include "listener_test.h"
#include "network/listener.h"
#include "pkt/dhcp.h"
#include "pkt/generate.h"

void
dhcpNetworkReceiveBootRequestPktTest()
{
  int port = 67;
  int pktType = DHCPREQUEST;
  int dhcpSocket = dhcpNetworkSocketInit (port);

  //socklen_t dhcpClientAddressLen;

  dhcpNetworkPktInfo_t
  getReplyDependencies (pktDhcpPacket_t *requestPkt)
  {
    char *dhcpClientAddress = TEST_FAKE_DATA_DHCP_NETWORK_YOUR_IP_ADDRESS;

    char *serverIdentifier = TEST_FAKE_DATA_DHCP_NETWORK_SERVER_IDENTIFIER;

    int leaseTime = TEST_FAKE_DATA_DHCP_NETWORK_IP_ADDRESS_LEASE_TIME;

    char *subnet = TEST_FAKE_DATA_DHCP_NETWORK_SUBNET_MASK;

    char *router = TEST_FAKE_DATA_DHCP_NETWORK_ROUTER;

    char *domain = TEST_FAKE_DATA_DHCP_NETWORK_DOMAIN_NAME;

    dhcpNetworkPktInfo_t info =
    {
      .fields = {
        {.func = (pktGenCallbackFunc_t)pktGenFieldYourIpAddress, .param = dhcpClientAddress},
        PKT_GEN_CALLBACK_NULL,
      },

      .options =
      {
        {.func = (pktGenCallbackFunc_t)pktGenOptDhcpServerIdentifier, .param = serverIdentifier},
        {.func = (pktGenCallbackFunc_t)pktGenOptIpAddrLeaseTime, .param = (void *)leaseTime},
        {.func = (pktGenCallbackFunc_t)pktGenOptSubnetMask, .param = subnet},
        {.func = (pktGenCallbackFunc_t)pktGenOptRouter, .param = router},
        {.func = (pktGenCallbackFunc_t)pktGenOptDomainName, .param = domain},
        PKT_GEN_CALLBACK_NULL,
      }
    };

    (void *)requestPkt;

    return info;
  }

  pktGenOffer (requestPkt, previousReplyPkt, info);

  fdReturnedValue = recvfrom (dhcpSocket, requestPkt, DHCP_PACKET_MAX_LEN, 0,
                              (struct sockaddr *)dhcpClientAddress, socklen_t *dhcpClientAddressLen);
// ادرس و طول ادرس از کجا داره میاد

  while (pktGetDhcpMessageType (requestPkt) != pktType)
    {
      if (fdReturnedValue >= 0 && previousReplyPkt != NULL && pktType = DHCPREQUEST
          && requestPkt->xid == previousReplyPkt->xid)
        return 0;

      else
        continue;
    }
}

