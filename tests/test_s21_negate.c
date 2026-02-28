#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"

// Тест 0
START_TEST(test_negate_0) {
  s21_decimal v1;
  from_str_to_decimal("7.9228162514264337593543950335", &v1);
  s21_decimal v2;
  s21_negate(v1, &v2);
  s21_decimal exp;
  from_str_to_decimal("-7.9228162514264337593543950335", &exp);
  int eq = s21_is_equal(v2, exp);
  ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 0
START_TEST(test_negate_1) {
  s21_decimal v1;
  from_str_to_decimal("-7.1228162514264337593543950335", &v1);
  s21_decimal v2;
  s21_negate(v1, &v2);
  s21_decimal exp;
  from_str_to_decimal("7.1228162514264337593543950335", &exp);

  print_decimal(v2);
  printf("\n");
  print_decimal(exp);
  printf("\n");

  int eq = s21_is_equal(v2, exp);
  ck_assert_int_eq(eq, 1);
}
END_TEST

Suite *negate_suite(void) {
  Suite *s = suite_create("negate_tests");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_negate_0);
  tcase_add_test(tc_core, test_negate_1);

  suite_add_tcase(s, tc_core);
  return s;
}

// Запуск тестов
int main_s21_negate(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = negate_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
