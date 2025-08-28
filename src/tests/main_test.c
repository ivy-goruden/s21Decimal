#include "main_test.h"

#include <check.h>
#include <stdio.h>

#include "../s21_decimal.h"
#include "../utilities.h"

void print_decimal_info(s21_decimal num) {
    printf("scale: %d\n", get_scale(num));
    printf("sign: %d (%c)\n", get_sign(num), get_sign(num) == 1 ? '-' : '+');
    printf("is zero: %s\n", is_zero(num) ? "yes" : "no");
}

void print_big_decimal_info(s21_big_decimal num) {
    printf("scale: %d\n", big_get_scale(num));
    printf("sign: %d (%c)\n", big_get_sign(num),
           big_get_sign(num) == 1 ? '-' : '+');
    printf("is zero: %s\n", big_is_zero(num) ? "yes" : "no");
}

int main(void) {

    // main_zero();
    main_s21_add();
    // main_s21_sub();
    // main_s21_mul();
    // main_s21_div();
    // main_s21_decimal_to_int();
    // main_s21_float_to_decimal();
}
