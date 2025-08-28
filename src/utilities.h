#ifndef UTILITIES_H
#define UTILITIES_H

#include "s21_decimal.h"

void print_decimal(s21_decimal num);
void print_big_decimal(s21_big_decimal num);
void create_decimal(s21_decimal *v, unsigned sign, unsigned scale, unsigned i0,
                    unsigned i1, unsigned i2);
void from_str_to_decimal(char *str, s21_decimal *dst);
int cmp_mantis_small(s21_decimal v1, s21_decimal v2);
void normalize_decimals(s21_decimal *a, s21_decimal *b);

#endif
