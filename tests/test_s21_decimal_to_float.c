#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"

// Тест 1
START_TEST(test_decimal_to_float_1) {
  s21_decimal v1;
  float act;
  from_str_to_decimal("7.9228162514264337593543950335", &v1);
  s21_from_decimal_to_float(v1, &act);

  char exp[20] = "7.922816e+00";
  char acts[20] = "";
  sprintf(acts, "%e", act);

  ck_assert_str_eq(acts, exp);
}
END_TEST

// Тест 2
START_TEST(test_decimal_to_float_2) {
  s21_decimal v1;
  float act;
  from_str_to_decimal("0.0000000000000000000000000001", &v1);
  s21_from_decimal_to_float(v1, &act);

  char exp[20] = "1.000000e-28";
  char acts[20] = "";
  sprintf(acts, "%e", act);

  ck_assert_str_eq(acts, exp);
}
END_TEST

Suite *decimal_to_float_suite(void) {
  Suite *s = suite_create("decimal_to_float_tests");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_decimal_to_float_1);
  tcase_add_test(tc_core, test_decimal_to_float_2);

  suite_add_tcase(s, tc_core);
  return s;
}

// Запуск тестов
int main_s21_decimal_to_float(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = decimal_to_float_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
