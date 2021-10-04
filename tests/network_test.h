#include "network/listener.h"

#if !defined(TESTS_NETWORK_H)
#define TESTS_NETWORK_H

#define TEST_FAKE_DATA_DHCP_NETWORK_YOUR_IP_ADDRESS            "192.168.133.117"

#define TEST_FAKE_DATA_DHCP_NETWORK_SERVER_IDENTIFIER        "192.168.133.30"
#define TEST_FAKE_DATA_DHCP_NETWORK_IP_ADDRESS_LEASE_TIME    600
#define TEST_FAKE_DATA_DHCP_NETWORK_SUBNET_MASK              "255.255.255.0"
#define TEST_FAKE_DATA_DHCP_NETWORK_ROUTER                   "192.168.1.1"
#define TEST_FAKE_DATA_DHCP_NETWORK_DOMAIN_NAME              "alireza.zharfpouyan.net"

int initSuiteNetwork();

int cleanupSuiteNetwork();

void dhcpNetworkListenerTest();

#endif
