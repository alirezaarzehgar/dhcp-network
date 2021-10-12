/**
 * @file listener_test.h
 * @author me
 * @brief unit test for network_listener
 * @version 0.1.0
 * @date 9 Oct 2021
 *
 * @copyright Copyright (c) 2021
 *
 */
#if !defined(LISTENER_TEST_H)
#define LISTENER_TEST_H

void dhcpNetworkReceiveBootRequestPktTest();

void dhcpNetworkReceiveDiscoveryPktTest();

void dhcpNetworkReceiveRequestPktTest();

void dhcpNetworkSendBootReplayPktTest();

void dhcpNetworkSocketInitTest();

void dhcpNetworkListenerTest();

#endif