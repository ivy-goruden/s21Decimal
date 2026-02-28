#include "main_test.h"

#include <check.h>
#include <stdio.h>

int main(void) {
  main_zero();
  main_s21_add();
  main_s21_sub();
  main_s21_mul();
  main_s21_div();
  main_s21_decimal_to_int();
  main_s21_float_to_decimal();
  main_s21_decimal_to_float();
  // **********************************************
  main_s21_equal();
  main_s21_not_equal();
  main_s21_less();
  main_s21_less_or_equal();
  main_s21_greater();
  main_s21_greater_or_equal();
  main_s21_floor();
  main_s21_round();
  main_s21_truncate();
  main_s21_negate();
}
