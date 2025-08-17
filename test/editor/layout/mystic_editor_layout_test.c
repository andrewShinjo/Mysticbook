#include "unity.h"
#include "editor/layout/mystic_editor_layout.h"

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void MysticEditorLineInitTest(void)
{
  MysticEditorLine line;
  const int result = MysticEditorLineInit(&line, 10);

  TEST_ASSERT_EQUAL_INT(line.x_start, -1);
  TEST_ASSERT_EQUAL_INT(line.y_start, -1);
  TEST_ASSERT_EQUAL_UINT16(line.height, 0);

  TEST_ASSERT_NOT_NULL(line.chars);
  TEST_ASSERT_NOT_NULL(line.x_positions);

  TEST_ASSERT_EQUAL_INT(line.length, 0);
  TEST_ASSERT_EQUAL_INT(line.capacity, 10);

  TEST_ASSERT_EQUAL_INT(line.dirty_from, -1);

  TEST_ASSERT_EQUAL_INT(result, 0);
}

// not needed when using generate_test_runner.rb
int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(MysticEditorLineInitTest);
  return UNITY_END();
}
