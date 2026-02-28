#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"
START_TEST(test_is_less_0) {
  s21_decimal v1;
  create_decimal(&v1, 0, 1, 0, 0, 0);
  s21_decimal v2;
  create_decimal(&v2, 0, 1, 0, 0, 1);
  int result = s21_is_less(v1, v2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_less_1) {
  s21_decimal v1;
  create_decimal(&v1, 0, 1, 1, 0, 0);
  s21_decimal v2;
  create_decimal(&v2, 0, 1, 0, 2, 0);
  int result = s21_is_less(v1, v2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_less_2) {
  s21_decimal v1;
  create_decimal(&v1, 0, 1, 1, 0, 0);
  s21_decimal v2;
  create_decimal(&v2, 0, 1, 1, 0, 0);
  int result = s21_is_less(v1, v2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_is_less_3) {
  s21_decimal v1;
  create_decimal(&v1, 1, 1, 1, 0, 0);
  s21_decimal v2;
  create_decimal(&v2, 0, 1, 1, 0, 0);
  int result = s21_is_less(v1, v2);
  ck_assert_int_eq(result, 1);
}
END_TEST

Suite *il_suite(void) {
  Suite *s = suite_create("is_less_tests");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_is_less_0);
  tcase_add_test(tc_core, test_is_less_1);
  tcase_add_test(tc_core, test_is_less_2);
  tcase_add_test(tc_core, test_is_less_3);

  suite_add_tcase(s, tc_core);
  return s;
}

// Запуск тестов
int main_s21_less() {
  Suite *s;
  int number_failed;
  SRunner *sr;
  s = il_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}