#include "utilities.h"

#include <stdio.h>
#include <string.h>

#include "s21_decimal.h"

void print_bits_chank(int chank) {
    int b = 0;
    char str[sizeof(int) * 8];
    for (int k = 0; k < 4; k++) {
        unsigned char byte = (char)(chank >> (k * 8));
        for (int i = 0; i < 8; i++) {
            unsigned char ch = byte & 1;
            str[b] = ch + '0';
            byte = byte >> 1;
            b++;
        }
    }
    for (int i = 0; i < (int)sizeof(str); i++) {
        printf("%c", str[i]);
        if ((i + 1) % 8 == 0) printf(" | ");
    }
}

void print_decimal(s21_decimal num) {
    int size = sizeof(num.bits) / sizeof(num.bits[0]);
    for (int i = 0; i < size; i++) {
        printf("bits %d: ", i);
        print_bits_chank(num.bits[i]);
        printf("\n");
    }
}

// инициализировать s21_decimal
// указать знак, порядок и первые 3 bits
void create_decimal(s21_decimal *v, unsigned sign, unsigned scale, unsigned i0,
                    unsigned i1, unsigned i2) {
    null_decimal(v);
    set_scale(v, scale);
    set_sign(v, sign);
    v->bits[0] = i0;
    v->bits[1] = i1;
    v->bits[2] = i2;
}

void from_str_to_decimal(char *str, s21_decimal *dst) {
    null_decimal(dst);

    int sign = 0;
    // int value = 0;
    int exp = 0;
    int scale = 0;
    int scale_sign = 0;

    int i = 0;
    for (; i < (int)strlen(str); i++) {
        if (str[i] >= '0' && str[i] <= '9') break;
        if (str[i] == '-') sign = 1;
    }
    s21_big_decimal tmp;
    big_null_decimal(&tmp);
    int dot = 0;
    for (; i < (int)strlen(str); i++) {
        if (str[i] == 'e' || str[i] == 'E') break;
        if (str[i] == '.') dot = 1;
        if (str[i] >= '0' && str[i] <= '9') {
            mul_10(&tmp);
            s21_big_decimal num;
            big_null_decimal(&num);
            num.bits[0] = str[i] - '0';
            bitwise_addition(tmp, num, &tmp);
            if (dot) exp++;
        }
    }
    for (; i < (int)strlen(str); i++) {
        if (str[i] == '-') scale_sign = 1;
        if (str[i] >= '0' && str[i] <= '9') {
            scale *= 10;
            scale += str[i] - '0';
        }
    }
    if (scale_sign) scale = -scale;
    scale -= exp;

    big_set_sign(&tmp, sign);
    if (scale > 0) {
        for (int i = 0; i < scale; i++) mul_10(&tmp);
    } else {
        big_set_scale(&tmp, -scale);
    }

    *dst = to_decimal(tmp);
}

// int cmp_mantis_small(s21_decimal v1, s21_decimal v2) {
//     int sizeb = (sizeof(s21_decimal) - sizeof(unsigned)) * 8;
//     int result = 0;
//     for (int i = sizeb - 1; i >= 0; i--) {
//         int b1 = get_bit(v1, i);
//         int b2 = get_bit(v2, i);
//         if (b1 != b2) {
//             if (b1 > b2) {
//                 result = 1;
//             } else if (b1 < b2) {
//                 result = -1;
//             }
//             break;
//         }
//     }
//     return result;
// }

// void normalize_decimals(s21_decimal *a, s21_decimal *b) {
//     int scale_a = get_scale(*a);
//     int scale_b = get_scale(*b);

//     s21_big_decimal a1 = tobig_decimal(*a);
//     s21_big_decimal b1 = tobig_decimal(*b);
//     while (scale_a != scale_b) {
//         if (scale_a < scale_b) {
//             mul_10(&a1);
//             scale_a++;
//         } else {
//             mul_10(&b1);
//             scale_b++;
//         }
//     }
//     *a = to_decimal(a1);
//     *b = to_decimal(b1);
// }

void normalize_decimalsCompare(s21_decimal *a, s21_decimal *b,
                               s21_big_decimal *a1, s21_big_decimal *b1) {
    int scale_a = get_scale(*a);
    int scale_b = get_scale(*b);

    s21_big_decimal a2 = tobig_decimal(*a);
    s21_big_decimal b2 = tobig_decimal(*b);
    while (scale_a != scale_b) {
        if (scale_a < scale_b) {
            mul_10(&a2);
            scale_a++;
        } else {
            mul_10(&b2);
            scale_b++;
        }
    }
    *a1 = a2;
    *b1 = b2;
}

void bank_round(s21_big_decimal *src, int remainder) {
    s21_big_decimal one;
    big_null_decimal(&one);
    one.bits[0] = 1;
    if (remainder == 5) {
        if (src->bits[0] & 1) {
            // нечетная, надо прибавить 1
            bitwise_addition(*src, one, src);
        }
    } else if (remainder > 5) {
        // остаток > 5, надо прибавить 1
        bitwise_addition(*src, one, src);
    }
}

int fit_to_decimal(s21_big_decimal src, s21_decimal *dst) {
    null_decimal(dst);

    int exit_code = OK;

    int remainder = 0;
    int scale = big_get_scale(src);
    while (scale > MAX_SCALE ||
           (scale > 0 && (src.bits[3] != 0 || src.bits[4] != 0 ||
                          src.bits[5] != 0 || src.bits[6] != 0))) {
        remainder = rem_10(src, &src);
        big_set_scale(&src, --scale);
    }

    bank_round(&src, remainder);
    set_sign(dst, big_get_sign(src));
    if (src.bits[3] != 0 || src.bits[4] != 0 || src.bits[5] != 0 ||
        src.bits[6] != 0) {
        exit_code = TOO_SMALL_ERROR;
        if (big_get_sign(src) == 0) {
            exit_code = TOO_BIG_ERROR;
        }
    } else {
        dst->bits[0] = src.bits[0];
        dst->bits[1] = src.bits[1];
        dst->bits[2] = src.bits[2];
        set_scale(dst, big_get_scale(src));
    }

    return exit_code;
}
