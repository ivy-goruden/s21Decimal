#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"

// Тест 1
START_TEST(test_decimal_to_int_1) {
  s21_decimal v1;
  int act;
  int exp = -2147483648;
  from_str_to_decimal("-2147483648", &v1);
  s21_from_decimal_to_int(v1, &act);

  ck_assert_int_eq(act, exp);
}
END_TEST

// Тест 2
START_TEST(test_decimal_to_int_2) {
  s21_decimal v1;
  int act;
  int exp = -7;
  from_str_to_decimal("-7.9228162514264337593543950335", &v1);
  s21_from_decimal_to_int(v1, &act);

  ck_assert_int_eq(act, exp);
}
END_TEST

// Тест 3
START_TEST(test_decimal_to_int_3) {
  s21_decimal v1;
  int act;
  int exp = 1;
  int result_exp = 1;
  from_str_to_decimal("-79228162514264337593543950335", &v1);
  int result = s21_from_decimal_to_int(v1, &act);

  ck_assert_int_eq(act, exp);
  ck_assert_int_eq(result, result_exp);
}
END_TEST

Suite *decimal_to_int_suite(void) {
  Suite *s = suite_create("decimal_to_int_tests");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_decimal_to_int_1);
  tcase_add_test(tc_core, test_decimal_to_int_2);
  tcase_add_test(tc_core, test_decimal_to_int_3);

  suite_add_tcase(s, tc_core);
  return s;
}

// Запуск тестов
int main_s21_decimal_to_int(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = decimal_to_int_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
