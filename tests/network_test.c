/**
 * @file network_test.c
 * @author alirezaarzehgar (alirezaarzehgar82@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-03
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "network_test.h"

int
initSuiteNetwork()
{
  return 0;
}

int
cleanupSuiteNetwork()
{
  return 0;
}

dhcpNetworkPktInfo_t
getReplyDependencies (pktDhcpPacket_t *discovery)
{
  dhcpNetworkPktInfo_t info =
  {
    .fields = {
      {.func = (pktGenCallbackFunc_t)pktGenFieldYourIpAddress, .param = TEST_FAKE_DATA_DHCP_NETWORK_YOUR_IP_ADDRESS},
      PKT_GEN_CALLBACK_NULL,
    },

    .options =
    {
      {.func = (pktGenCallbackFunc_t)pktGenOptDhcpServerIdentifier, .param = TEST_FAKE_DATA_DHCP_NETWORK_SERVER_IDENTIFIER},
      {.func = (pktGenCallbackFunc_t)pktGenOptIpAddrLeaseTime, .param = (void *)TEST_FAKE_DATA_DHCP_NETWORK_IP_ADDRESS_LEASE_TIME},
      {.func = (pktGenCallbackFunc_t)pktGenOptSubnetMask, .param = TEST_FAKE_DATA_DHCP_NETWORK_SUBNET_MASK},
      {.func = (pktGenCallbackFunc_t)pktGenOptRouter, .param = TEST_FAKE_DATA_DHCP_NETWORK_ROUTER},
      {.func = (pktGenCallbackFunc_t)pktGenOptDomainName, .param = TEST_FAKE_DATA_DHCP_NETWORK_DOMAIN_NAME},
      PKT_GEN_CALLBACK_NULL,
    }
  };

  (void *)discovery;      /* Quit unused warning */

  return info;
}

char *
leaseOperation (pktDhcpPacket_t *ack)
{
  printf ("lease!\n");
  return NULL;
}

/**
 * @brief Need to system test.
 * We use it just like an endpoint
 *
 */
void
dhcpNetworkListenerTest()
{
  int retval;

  retval = dhcpNetworkListener ("192.168.133.30", 67, getReplyDependencies,
                                leaseOperation);

  if (retval)
    perror ("dhcpNetworkListener");
}