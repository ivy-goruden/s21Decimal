#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"

// Тест 1
START_TEST(test_float_to_decimal_1) {
  float value = -1.234568e-24;
  s21_decimal act;
  s21_decimal exp;
  from_str_to_decimal("-0.0000000000000000000000012346", &exp);
  int result = s21_from_float_to_decimal(value, &act);
  int result_exp = 0;

  int eq = s21_is_equal(act, exp);
  ck_assert_int_eq(eq, 1);
  ck_assert_int_eq(result, result_exp);
}
END_TEST

// Тест 2
START_TEST(test_float_to_decimal_2) {
  float value = 1.234568e-29;
  s21_decimal act;
  int result = s21_from_float_to_decimal(value, &act);
  int result_exp = 1;

  ck_assert_int_eq(result, result_exp);
}
END_TEST

// Тест 3
START_TEST(test_float_to_decimal_3) {
  float value = 7.932819e+28;
  s21_decimal act;
  int result = s21_from_float_to_decimal(value, &act);
  int result_exp = 1;

  ck_assert_int_eq(result, result_exp);
}
END_TEST

// Тест 4
START_TEST(test_float_to_decimal_4) {
  float value = 9.000000e-29;
  s21_decimal act;
  int result = s21_from_float_to_decimal(value, &act);
  int result_exp = 1;

  ck_assert_int_eq(result, result_exp);
}
END_TEST

Suite *float_to_decimal_suite(void) {
  Suite *s = suite_create("float_to_decimal_tests");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_float_to_decimal_1);
  tcase_add_test(tc_core, test_float_to_decimal_2);
  tcase_add_test(tc_core, test_float_to_decimal_3);
  tcase_add_test(tc_core, test_float_to_decimal_4);

  suite_add_tcase(s, tc_core);
  return s;
}

// Запуск тестов
int main_s21_float_to_decimal(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = float_to_decimal_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
