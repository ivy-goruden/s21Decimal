#ifndef DECIMAL_H
#define DECIMAL_H

typedef struct {
    unsigned bits[4];
} s21_decimal;

typedef struct {
    unsigned bits[8];
} s21_big_decimal;

#define OK 0
#define CONV_ERROR 1
#define CALC_ERROR 1

s21_big_decimal tobig_decimal(s21_decimal num);
s21_decimal to_decimal(s21_big_decimal big);

int is_zero(s21_decimal num);
void null_decimal(s21_decimal *num);
int get_sign(s21_decimal num);
void set_sign(s21_decimal *num, int sign_value);
int get_scale(s21_decimal num);
void set_scale(s21_decimal *num, unsigned char scale_value);
int get_bit(s21_decimal num, int bit);
void set_bit(s21_decimal *num, unsigned bit, unsigned char value);

s21_big_decimal tobig_decimal(s21_decimal num);
int big_is_zero(s21_big_decimal num);
void big_null_decimal(s21_big_decimal *num);
int big_get_sign(s21_big_decimal num);
void big_set_sign(s21_big_decimal *num, int sign_value);
int big_get_scale(s21_big_decimal num);
void big_set_scale(s21_big_decimal *num, unsigned char scale_value);
int big_get_bit(s21_big_decimal num, int bit);
void big_set_bit(s21_big_decimal *num, unsigned bit, unsigned char value);

void shift_left(s21_big_decimal *num);
void shift_left_num(s21_big_decimal *num, int shift_value);

void mul_10(s21_big_decimal *num);
void enlarge_scale(s21_big_decimal *num, unsigned char scale);
void bitwise_addition(s21_big_decimal v1, s21_big_decimal v2,
                      s21_big_decimal *result);
void bitwise_substruction(s21_big_decimal v1, s21_big_decimal v2,
                          s21_big_decimal *result);

int compare(s21_decimal a, s21_decimal b);

int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif
