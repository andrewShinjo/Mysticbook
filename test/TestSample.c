#include "unity.h"
#include "unity_internals.h"

void setUp(void)
{
  // Set up code here
}

void tearDown(void)
{
  // Clean up code here
}

void test_UnityWorks(void)
{
  TEST_ASSERT_EQUAL_INT(1, 1);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_UnityWorks);
  return UNITY_END();
}
