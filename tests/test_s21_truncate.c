#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"

START_TEST(test_truncate_positive) {
  s21_decimal value, result, expected;
  // 123.456
  create_decimal(&value, 0, 3, 123456, 0, 0);
  // 123
  create_decimal(&expected, 0, 0, 123, 0, 0);

  int status = s21_truncate(value, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_truncate_negative) {
  s21_decimal value, result, expected;
  // -123.456
  create_decimal(&value, 1, 3, 123456, 0, 0);
  // -123
  create_decimal(&expected, 1, 0, 123, 0, 0);

  int status = s21_truncate(value, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_truncate_zero_scale) {
  s21_decimal value, result, expected;
  // 123 (scale 0)
  create_decimal(&value, 0, 0, 123, 0, 0);
  // Should remain unchanged
  create_decimal(&expected, 0, 0, 123, 0, 0);

  int status = s21_truncate(value, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_truncate_large_number) {
  s21_decimal value, result, expected;
  // ~7.9e28 with scale 1
  create_decimal(&value, 0, 1, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
  // ~7.9e28
  create_decimal(&expected, 0, 0, 0x99999999, 0x99999999, 0x19999999);
  int status = s21_truncate(value, &result);
  // print_decimal(result);
  // printf("%c", '\n');
  // print_decimal(expected);
  // printf("%c", '\n');
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_truncate_small_fraction) {
  s21_decimal value, result, expected;
  // 1e-28
  create_decimal(&value, 0, 28, 1, 0, 0);
  // 0
  create_decimal(&expected, 0, 0, 0, 0, 0);
  int status = s21_truncate(value, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_truncate_max_scale) {
  s21_decimal value, result, expected;
  create_decimal(&value, 0, 28, 123456789, 0, 0);
  create_decimal(&expected, 0, 0, 0, 0, 0);
  int status = s21_truncate(value, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_truncate_null_pointer) {
  s21_decimal value;
  // 12.3
  create_decimal(&value, 0, 1, 123, 0, 0);

  int status = s21_truncate(value, NULL);
  ck_assert_int_eq(status, 1); // Should return error
}
END_TEST

Suite *truncate_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("truncate");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_truncate_positive);
  tcase_add_test(tc_core, test_truncate_negative);
  tcase_add_test(tc_core, test_truncate_zero_scale);
  tcase_add_test(tc_core, test_truncate_large_number);
  tcase_add_test(tc_core, test_truncate_small_fraction);
  tcase_add_test(tc_core, test_truncate_max_scale);
  tcase_add_test(tc_core, test_truncate_null_pointer);

  suite_add_tcase(s, tc_core);

  return s;
}

int main_s21_truncate(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = truncate_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}