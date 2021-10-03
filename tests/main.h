#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "network_test.h"

#if !defined(TESTS_MAIN_H)
#define TESTS_MAIN_H

CU_TestInfo network_tests[] =
{
  {"dhcpNetworkListenerTest", dhcpNetworkListenerTest},
  CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] =
{
  {
    .pName = "network suite",
    .pInitFunc = initSuiteNetwork,
    .pCleanupFunc = cleanupSuiteNetwork,
    .pTests = network_tests,
  },
  CU_SUITE_INFO_NULL,
};

#endif
