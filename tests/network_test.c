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
getOfferDependencies (pktDhcpPacket_t *discovery)
{
  dhcpNetworkPktInfo_t info =
  {
    .fields = {
      {.func = (pktGenCallbackFunc_t)pktGenFieldYourIpAddress, .param = "192.168.133.117"},
      PKT_GEN_CALLBACK_NULL,
    },

    .options =
    {
      {.func = (pktGenCallbackFunc_t)pktGenOptDhcpServerIdentifier, .param = "192.168.133.30"},
      {.func = (pktGenCallbackFunc_t)pktGenOptIpAddrLeaseTime, .param = (void *)600},
      {.func = (pktGenCallbackFunc_t)pktGenOptSubnetMask, .param = "255.255.255.0"},
      {.func = (pktGenCallbackFunc_t)pktGenOptRouter, .param = "192.168.1.1"},
      {.func = (pktGenCallbackFunc_t)pktGenOptDomainName, .param = "alireza.zharfpouyan.net"},
      PKT_GEN_CALLBACK_NULL,
    }
  };

  (void *)discovery;      /* Quit unused warning */

  return info;
}

dhcpNetworkPktInfo_t
getAckDependencies (pktDhcpPacket_t *request)
{
  dhcpNetworkPktInfo_t info =
  {
    .fields = {
      {.func = (pktGenCallbackFunc_t)pktGenFieldYourIpAddress, .param = "192.168.133.117"},
      PKT_GEN_CALLBACK_NULL,
    },

    .options =
    {
      {.func = (pktGenCallbackFunc_t)pktGenOptDhcpServerIdentifier, .param = "192.168.133.30"},
      {.func = (pktGenCallbackFunc_t)pktGenOptIpAddrLeaseTime, .param = (void *)600},
      {.func = (pktGenCallbackFunc_t)pktGenOptSubnetMask, .param = "255.255.255.0"},
      {.func = (pktGenCallbackFunc_t)pktGenOptRouter, .param = "192.168.1.1"},
      {.func = (pktGenCallbackFunc_t)pktGenOptDomainName, .param = "alireza.zharfpouyan.net"},
      PKT_GEN_CALLBACK_NULL,
    }
  };

  (void *)request;      /* Quit unused warning */

  return info;
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

  retval = dhcpNetworkListener ("192.168.133.30", 67, getOfferDependencies,
                                getAckDependencies);

  if (retval)
    perror ("dhcpNetworkListener");
}