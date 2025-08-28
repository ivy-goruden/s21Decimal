#include "main_test.h"

#include <check.h>
#include <stdio.h>

#include "../../s21_decimal.h"
#include "../../utilities.h"
int main(void) {
  Suite *s;
  s = add_suite();
  main_s21_equal(s);
  main_s21_not_equal(s);
  main_s21_less(s);
  main_s21_less_or_equal(s);
  main_s21_greater(s);
  main_s21_greater_or_equal(s);
}
