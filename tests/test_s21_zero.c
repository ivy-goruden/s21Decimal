#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"

// Тест 1: проверка на ноль
START_TEST(test_is_zero_1) {
  s21_decimal num;
  num.bits[0] = 0;
  num.bits[1] = 0;
  num.bits[2] = 0;
  num.bits[3] = 1;
  int result = is_zero(num);

  ck_assert_int_ne(result, 0);
}
END_TEST

// Тест 2: проверка на не ноль
START_TEST(test_is_zero_2) {
  s21_decimal num;
  num.bits[0] = 1;
  num.bits[1] = 0;
  num.bits[2] = 0;
  num.bits[3] = 1;
  int result = is_zero(num);

  ck_assert_int_eq(result, 0);
}
END_TEST

Suite *zero_suite(void) {
  Suite *s = suite_create("zero_tests");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_is_zero_1);
  tcase_add_test(tc_core, test_is_zero_2);

  suite_add_tcase(s, tc_core);
  return s;
}

// Запуск тестов
int main_zero(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = zero_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
