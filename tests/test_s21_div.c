#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"

// Тест 1
START_TEST(test_div_1) {
  s21_decimal v1;
  s21_decimal v2;
  s21_decimal act;
  s21_decimal exp;
  from_str_to_decimal("-7.9228162514264337593543950335", &v1);
  from_str_to_decimal("0.00000000000004", &v2);
  from_str_to_decimal("-198070406285660.84398385987584", &exp);
  s21_div(v1, v2, &act);

  int eq = s21_is_equal(act, exp);
  ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 2
START_TEST(test_div_2) {
  s21_decimal v1;
  s21_decimal v2;
  s21_decimal act;
  s21_decimal exp;
  from_str_to_decimal("-0.00879", &v1);
  from_str_to_decimal("0.123456", &v2);
  from_str_to_decimal("-0.0711994556765163297045101089", &exp);
  s21_div(v1, v2, &act);

  int eq = s21_is_equal(act, exp);
  ck_assert_int_eq(eq, 1);
}
END_TEST

Suite *div_suite(void) {
  Suite *s = suite_create("div_tests");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_div_1);
  tcase_add_test(tc_core, test_div_2);

  suite_add_tcase(s, tc_core);
  return s;
}

// Запуск тестов
int main_s21_div(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = div_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
