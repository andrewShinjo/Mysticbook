#include <stdbool.h>
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

void MysticEditorLineInitNegativeTest(void)
{
  MysticEditorLine line;
  const int result = MysticEditorLineInit(&line, 0);
  TEST_ASSERT_EQUAL_INT(result, 1);

  MysticEditorLine line2;
  const int result2 = MysticEditorLineInit(&line2, -1);
  TEST_ASSERT_EQUAL_INT(result2, 1);
}

void MysticEditorLineInsertAtTest(void)
{
  MysticEditorLine line;
  MysticEditorLineInit(&line, 16);

  TEST_ASSERT_EQUAL_INT(line.length, 0);
  TEST_ASSERT_EQUAL_INT(line.capacity, 16);

  // Test case 1: insert "test" at index=0 to a line with "".
  // Expected output: "test"
  bool result = MysticEditorLineInsertAt(&line, 0, "test", 4);
  const char *expected_chars = "test";

  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(line.chars, expected_chars, 4);
  TEST_ASSERT_EQUAL_INT(line.length, 4);

  // Test case 2: insert "A" at index=4 to a line with "test".
  // Expected output: "testA"
  bool result2 = MysticEditorLineInsertAt(&line, 4, "A", 1);
  const char *expected_chars2 = "testA";

  TEST_ASSERT_TRUE(result2);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(line.chars, expected_chars2, 5);
  TEST_ASSERT_EQUAL_INT(line.length, 5);

  // Test case 3: insert "BB" at index=1 to a line with "testA".
  // Expected output: "tBBestA"
  bool result3 = MysticEditorLineInsertAt(&line, 1, "BB", 2);
  const char *expected_chars3 = "tBBestA";

  TEST_ASSERT_TRUE(result3);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(line.chars, expected_chars3, 7);
  TEST_ASSERT_EQUAL_INT(line.length, 7);

  // Test case 4: n = 0 succeeds by doing nothing.
  bool result4 = MysticEditorLineInsertAt(&line, 7, "C", 0);

  TEST_ASSERT_TRUE(result4);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(line.chars, expected_chars3, 7);
  TEST_ASSERT_EQUAL_INT(line.length, 7);
}

void MysticEditorLineInsertAtNegativeTest(void)
{
  // Test case 1: line is NULL.
  // Expected output: failure
  MysticEditorLine *line = NULL;
  bool result = MysticEditorLineInsertAt(line, 0, "test", 4);
  TEST_ASSERT_FALSE(result);

  // Test case 2: s is NULL.
  // Expected output: failure
  MysticEditorLine line2;
  MysticEditorLineInit(&line2, 16);
  bool result2 = MysticEditorLineInsertAt(&line2, 0, NULL, 4);
  TEST_ASSERT_FALSE(result2);

  // Test case 3: index == capacity.
  // Expected output: failure
  MysticEditorLine line3;
  MysticEditorLineInit(&line3, 16);
  bool result3 = MysticEditorLineInsertAt(
    &line3,
    16,
    "test",
    4);
  TEST_ASSERT_FALSE(result3);

  // Test case 4: index > capacity.
  // Expected output: failure
  MysticEditorLine line4;
  MysticEditorLineInit(&line4, 16);
  bool result4 = MysticEditorLineInsertAt(
    &line4,
    17,
    "test",
    4);
  TEST_ASSERT_FALSE(result4);

  // Test case 5: self->length + n > capacity
  // Expected output: failure
  MysticEditorLine line5;
  MysticEditorLineInit(&line5, 16);
  TEST_ASSERT_TRUE(
      MysticEditorLineInsertAt(
      &line5,
      0,
      "aaaaaaaa",
      8));
  TEST_ASSERT_FALSE(
    MysticEditorLineInsertAt(
      &line5,
      0,
      "bbbbbbbbb",
      9));
}

// not needed when using generate_test_runner.rb
int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(MysticEditorLineInitTest);
  RUN_TEST(MysticEditorLineInitNegativeTest);
  RUN_TEST(MysticEditorLineInsertAtTest);
  RUN_TEST(MysticEditorLineInsertAtNegativeTest);
  return UNITY_END();
}
