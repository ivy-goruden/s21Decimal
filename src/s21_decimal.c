#include "s21_decimal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"

// конвертация decimal -> big_decimal
s21_big_decimal tobig_decimal(s21_decimal num) {
  s21_big_decimal big;
  big_null_decimal(&big);
  big.bits[0] = num.bits[0];
  big.bits[1] = num.bits[1];
  big.bits[2] = num.bits[2];
  big.bits[7] = num.bits[3];
  return big;
}

// конвертация big_decimal -> decimal
// из big_decimal учитываются только первые 3 bits
s21_decimal to_decimal(s21_big_decimal big) {
  s21_decimal num;
  null_decimal(&num);
  num.bits[0] = big.bits[0];
  num.bits[1] = big.bits[1];
  num.bits[2] = big.bits[2];
  num.bits[3] = big.bits[7];
  return num;
}

// проверка decimal на ноль
int is_zero(s21_decimal num) { return big_is_zero(tobig_decimal(num)); }

// очистить decimal
void null_decimal(s21_decimal *num) {
  memset(&(num->bits), 0, sizeof(num->bits));
}

// вернуть знак decimal, 0 -> плюс, 1 -> минус
int get_sign(s21_decimal num) { return get_bit(num, 127); }

// поставить знак decimal, 0 -> плюс, 1 -> минус
void set_sign(s21_decimal *num, int sign_value) {
  set_bit(num, 127, sign_value & 1);
}

// вернуть порядок decimal
int get_scale(s21_decimal num) { return (num.bits[3] >> 16) & (int)0xff; }

// поставить порядок decimal
void set_scale(s21_decimal *num, unsigned char scale_value) {
  if (scale_value > 28) scale_value = 28;
  unsigned *pint = (unsigned *)num->bits + 3;
  unsigned char *pbyte = (unsigned char *)pint + 2;
  *pbyte = scale_value;
}

// вернуть значение бита decimal с номером от 0 до 127
int get_bit(s21_decimal num, int bit) {
  if (bit >= 96) bit += 128;
  return big_get_bit(tobig_decimal(num), bit);
}

// установить значение бита decimal с номером от 0 до 127
void set_bit(s21_decimal *num, unsigned bit, unsigned char value) {
  if (bit > 127) bit = 127;
  big_set_bit((s21_big_decimal *)num, bit, value);
}

int big_is_zero(s21_big_decimal num) {
  int bits = sizeof(s21_big_decimal) / sizeof(unsigned) - 1;

  int result = 1;
  for (int i = 0; i < bits; i++) {
    if (num.bits[i] != 0) {
      result = 0;
      break;
    }
  }
  return result;
}

void big_null_decimal(s21_big_decimal *num) {
  memset(&(num->bits), 0, sizeof(num->bits));
}

int big_get_sign(s21_big_decimal num) { return big_get_bit(num, 255); }

void big_set_sign(s21_big_decimal *num, int sign_value) {
  big_set_bit(num, 255, sign_value & 1);
}

int big_get_scale(s21_big_decimal num) {
  return (num.bits[7] >> 16) & (int)0xff;
}

void big_set_scale(s21_big_decimal *num, unsigned char scale_value) {
  if (scale_value > 57) scale_value = 57;
  unsigned *pint = (unsigned *)num->bits + 7;
  unsigned char *pbyte = (unsigned char *)pint + 2;
  *pbyte = scale_value;
}

// вернуть значение бита big_decimal с номером от 0 до 255
int big_get_bit(s21_big_decimal num, int bit) {
  int bite = bit / 8;
  int bit_int_bite = bit % 8;
  char *pbite = (char *)&num + bite;
  return (*pbite >> bit_int_bite) & 1;
}

// установить значение бита big_decimal с номером от 0 до 255
void big_set_bit(s21_big_decimal *num, unsigned bit, unsigned char value) {
  int bite = bit / 8;
  int bit_int_bite = (bit + 8) % 8;
  char *pbite = (char *)num + bite;
  if (value == 0) {
    *pbite &= ~(1 << bit_int_bite);
  } else {
    *pbite |= 1 << bit_int_bite;
  }
}

// сдвиг влево (умножить на 2) big_decimal на 1 бит
// сдвигаются только значащие биты, int в котором лежить порядок и знак не
// сдвигаются
void shift_left(s21_big_decimal *num) {
  unsigned bit31 = 0;
  for (int i = 0; i < 7; i++) {
    unsigned bits = num->bits[i];
    num->bits[i] <<= 1;
    num->bits[i] |= bit31;
    bit31 = bits >> 31;
  }
}

// сдвиг влево (умножить на 2^shift_value) big_decimal на shift_value бит
// сдвигаются только значащие биты, int в котором лежить порядок и знак не
// сдвигаются
void shift_left_num(s21_big_decimal *num, int shift_value) {
  if (shift_value < 0) shift_value = 0;
  if (shift_value > 224) shift_value = 224;
  for (int i = 0; i < shift_value; i++) {
    shift_left(num);
  }
}

void shift_right(s21_big_decimal *num) {
  for (int i = 0; i < 7; i++) {
    num->bits[i] >>= 1;
    if (i < 6) {
      num->bits[i] |= ((num->bits[i + 1] & 1) << 31);
    }
  }
}

void shift_right_num(s21_big_decimal *num, int shift_value) {
  for (int i = 0; i < shift_value; i++) {
    shift_right(num);
  }
}

// умножить num на 10. результат положить num
void mul_10(s21_big_decimal *num) {
  s21_big_decimal num1 = *num;
  s21_big_decimal num2 = *num;
  shift_left_num(&num1, 3);
  shift_left_num(&num2, 1);
  bitwise_addition(num1, num2, num);
}

void div_10(s21_big_decimal *num) {
  s21_big_decimal num1 = *num;
  s21_big_decimal num2 = *num;
  shift_right_num(&num1, 3);
  shift_right_num(&num2, 1);
  bitwise_addition(num1, num2, num);
}

// поставить big_decimal новый порядок, который задается параметром scale
void enlarge_scale(s21_big_decimal *num, unsigned char scale) {
  unsigned char s = big_get_scale(*num);
  for (int i = 0; i < (scale - s); i++) {
    mul_10(num);
  }
  big_set_scale(num, scale);
}

// сравнить два s21_big_decimal, только мантисы
// без учета порядка и знака
// v1 > v2 -> return 1
// v1 = v2 -> return 0
// v1 < v2 -> return -1
int cmp_mantis(s21_big_decimal v1, s21_big_decimal v2) {
  int sizeb = (sizeof(s21_big_decimal) - sizeof(unsigned)) * 8;
  int result = 0;
  for (int i = sizeb - 1; i >= 0; i--) {
    int b1 = big_get_bit(v1, i);
    int b2 = big_get_bit(v2, i);
    if (b1 != b2) {
      if (b1 > b2) {
        result = 1;
      } else if (b1 < b2) {
        result = -1;
      }
      break;
    }
  }
  return result;
}

// поиск ненулевого бита мантисы s21_big_decimal
int not_null(s21_big_decimal v) {
  int sizeb = (sizeof(s21_big_decimal) - sizeof(unsigned)) * 8;
  int not_null = -1;
  for (int i = sizeb - 1; i >= 0; i--) {
    if (big_get_bit(v, i)) {
      not_null = i;
      break;
    }
  }
  return not_null;
}

/*
    то что идет в текущий разряд
    3%2=1,  2%2=0,  1%2=1,  0%2=0
    то что идет в старший разряд = mem
    3/2=1,  2/2=1,  1/2=0,  0/2=0
*/
// побитовое сложение мантис двух big_decimal
// в результате не меняется порядок и знак
void bitwise_addition(s21_big_decimal v1, s21_big_decimal v2,
                      s21_big_decimal *result) {
  unsigned mem = 0;
  int sizeb = (sizeof(s21_big_decimal) - sizeof(unsigned)) * 8;
  for (int i = 0; i < sizeb; i++) {
    unsigned bit = big_get_bit(v1, i) + big_get_bit(v2, i) + mem;
    big_set_bit(result, i, bit % 2);
    mem = bit / 2;
  }
}

// побитовое вычитание мантис двух big_decimal. по умолчанию v1 > v2
// в результате не меняется порядок и знак
void bitwise_substruction(s21_big_decimal v1, s21_big_decimal v2,
                          s21_big_decimal *result) {
  // значащие байты
  int size = (sizeof(s21_big_decimal) - sizeof(unsigned));

  // дополнение вычитаемого v2 до 1 -> v2
  for (int i = 0; i < (size / sizeof(unsigned)); i++) {
    v2.bits[i] = ~v2.bits[i];
  }

  // прибавляем дополнение v2 к уменьшаемому v1 -> v1
  bitwise_addition(v1, v2, &v1);

  // делаем единицу -> one
  s21_big_decimal one;
  big_null_decimal(&one);
  big_set_bit(&one, 0, 1);

  // прибавляем v1 к one -> v1
  bitwise_addition(v1, one, &v1);

  // сохраняем мантису v1 в result
  memcpy(result, &(v1.bits), size);
}

/*
    Сравниваете длины чисел (если они разной длины, то число с большей длиной —
   больше). Если длины равны, идёте по битам слева направо. На первом
   отличающемся бите тот, у кого бит равен 1 — большее число. Если все биты
   одинаковые — числа равны.
*/
int compare(s21_decimal a, s21_decimal b) {
  // Приводим к одному масштабу (чтобы сравнивать мантиссы)
  normalize_decimals(&a, &b);
  // Получаем знаки
  int sign_a = get_sign(a);
  int sign_b = get_sign(b);

  // Случай 1: разные знаки
  if (sign_a != sign_b) {
    return sign_a ? 2
                  : 1;  // Если a отрицательное -> b больше (2), иначе наоборот
  }

  // Сравниваем мантиссы
  int cmp = cmp_mantis_small(a, b);

  // Оба положительные
  if (!sign_a) {
    if (cmp == 0) return 0;   // a == b
    return cmp == 1 ? 1 : 2;  // a > b → 1, иначе 2
  }
  // Оба отрицательные (обратная логика)
  else {
    if (cmp == 0) return 0;  // a == b
    return cmp == 1 ? 2
                    : 1;  // Если |a| > |b| → фактически a < b → возвращаем 2
  }
}

int s21_is_less(s21_decimal a, s21_decimal b) { return compare(a, b) == 2; }

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  int res = compare(a, b);
  return res == 2 || res == 0;
}

int s21_is_greater(s21_decimal a, s21_decimal b) { return compare(a, b) == 1; }

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  int res = compare(a, b);
  return res == 1 || res == 0;
}

// проверяет равенство двух decimal
// числа равны если все bits чисел тоже равны
// вернет 1 - если v1 == v2, иначе 0
int s21_is_equal(s21_decimal v1, s21_decimal v2) {
  int result = 1;
  for (int i = 0; i < 4; i++) {
    if (v1.bits[i] != v2.bits[i]) {
      result = 0;
      break;
    }
  }
  return result;
}

// проверяет не равенство двух decimal
// вернет 1 - если v1 <> v2, иначе 0
int s21_is_not_equal(s21_decimal v1, s21_decimal v2) {
  return s21_is_equal(v1, v2) == 0 ? 1 : 0;
}

// 1000/10^2 + 1000/10^3 = (1000 * 10^1) / (10^2 * 10^1) + 1000/10^3 =
// 10000/10^3 + 1000/10^3
// 1000 * 10 = 1000 * 8 + 1000 * 2 = 1000 * 2^3 + 1000 *
// 2^1 = 1000 << 3 + 1000 << 1 = 1000 -> decimal -> shift_left(3) + 1000 ->
// decimal -> shift_left(1) = 10000/10^3 10000/10^3 + 1000/10^3 -> 11000/10^3

// 1000 / 10 = 1000 / (8 + 2) = 1000 * 2^3 + 1000 *

// сложение двух s21_decimal
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // приведение к s21_big_decimal
  s21_big_decimal v1 = tobig_decimal(value_1);
  s21_big_decimal v2 = tobig_decimal(value_2);

  // нормализация, приведение к одинаковому порядку
  int v1s = big_get_scale(v1);
  int v2s = big_get_scale(v2);
  if (v1s > v2s) {
    enlarge_scale(&v2, v1s);
  } else if (v1s < v2s) {
    enlarge_scale(&v1, v2s);
  }

  s21_big_decimal res;
  big_null_decimal(&res);

  // сложение
  if (big_get_sign(v1) == 0 && big_get_sign(v2) == 0) {
    bitwise_addition(v1, v2, &res);
    big_set_sign(&res, 0);

    if (big_get_bit(res, 96)) {
    }

    printf("%d\n", v1s);
  } else if (big_get_sign(v1) == 1 && big_get_sign(v2) == 1) {
    bitwise_addition(v1, v2, &res);
    big_set_sign(&res, 1);
  } else {
    if (cmp_mantis(v1, v2) > 0) {
      bitwise_substruction(v1, v2, &res);
      // знак берем у большего
      big_set_sign(&res, big_get_sign(v1));
    } else {
      bitwise_substruction(v2, v1, &res);
      // знак берем у большего
      big_set_sign(&res, big_get_sign(v2));
    }
  }
  big_set_scale(&res, big_get_scale(v1));

  print_big_decimal(res);
  printf("\n");

  // копирование результата
  *result = to_decimal(res);

  return OK;
}

// вычитание двух s21_decimal
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // приведение к s21_big_decimal
  s21_big_decimal v1 = tobig_decimal(value_1);
  s21_big_decimal v2 = tobig_decimal(value_2);

  // нормализация, приведение к одинаковому порядку
  int v1s = big_get_scale(v1);
  int v2s = big_get_scale(v2);
  if (v1s > v2s) {
    enlarge_scale(&v2, v1s);
  } else if (v1s < v2s) {
    enlarge_scale(&v1, v2s);
  }

  s21_big_decimal res;
  big_null_decimal(&res);

  // вычитание
  if (big_get_sign(v1) == 1 && big_get_sign(v2) == 0) {
    bitwise_addition(v1, v2, &res);
    big_set_sign(&res, 1);
  } else if (big_get_sign(v1) == 0 && big_get_sign(v2) == 1) {
    bitwise_addition(v1, v2, &res);
    big_set_sign(&res, 0);
  } else {
    if (cmp_mantis(v2, v1) > 0) {
      bitwise_substruction(v2, v1, &res);
      // знак меняется на противополжный
      big_set_sign(&res, big_get_sign(v1) ? 0 : 1);
    } else {
      bitwise_substruction(v1, v2, &res);
      // знак сохраняется
      big_set_sign(&res, big_get_sign(v1));
    }
  }
  big_set_scale(&res, big_get_scale(v1));

  // копирование результата
  *result = to_decimal(res);

  return OK;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // результат
  s21_big_decimal res;
  big_null_decimal(&res);

  // приведение к s21_big_decimal
  s21_big_decimal v1 = tobig_decimal(value_1);
  s21_big_decimal v2 = tobig_decimal(value_2);

  // значащие байты
  int size = (sizeof(s21_big_decimal) - sizeof(unsigned));
  // значащие биты
  int sizeb = size * 8;
  // поиск ненулевого бита v2
  int not_null_i = not_null(v2);
  // перебор разрядов v2 начиная со старшего не нулевого
  for (int i = not_null_i; i >= 0; i--) {
    shift_left(&res);
    if (big_get_bit(v2, i)) {
      bitwise_addition(res, v1, &res);
    }
  }
  // знак (-) если один множитель со знаком (-) а другой нет
  big_set_sign(&res, big_get_sign(v1) ^ big_get_sign(v2));
  // порядок результата = порядок v1 + порядок v2
  big_set_scale(&res, big_get_scale(v1) + big_get_scale(v2));
  // копирование результата
  *result = to_decimal(res);

  return OK;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // результат
  s21_big_decimal res;
  big_null_decimal(&res);

  // приведение к s21_big_decimal
  s21_big_decimal v1 = tobig_decimal(value_1);
  s21_big_decimal v2 = tobig_decimal(value_2);

  // нормализация, scale 1 >= scale 2
  if (big_get_scale(v1) < big_get_scale(v2)) {
    enlarge_scale(&v1, big_get_scale(v2));
  }

  // делим если делимое больше делителя и делитель <> 0
  if (!big_is_zero(v2)) {
    // первый ненулевой бит v1
    int nnb1 = not_null(v1);
    // первый ненулевой бит v2
    int nnb2 = not_null(v2);
    // деление столбиком
    // последовательно находим остаток reminder и
    // максимально возможный делитель delitel для этого остатка
    s21_big_decimal reminder = v1;
    for (int i = nnb1 - nnb2; i >= 0; i--) {
      s21_big_decimal delitel = v2;
      shift_left_num(&delitel, i);
      if (cmp_mantis(reminder, delitel) >= 0) {
        shift_left(&res);
        big_set_bit(&res, 0, 1);

        bitwise_substruction(reminder, delitel, &reminder);
      } else {
        shift_left(&res);
      }
    }
  } else {
    // ошибка - на ноль делить нельзя
    return CALC_ERROR;
  }

  // знак (-) если один множитель со знаком (-) а другой нет
  big_set_sign(&res, big_get_sign(v1) ^ big_get_sign(v2));
  // порядок результата = порядок v1 - порядок v2
  big_set_scale(&res, big_get_scale(v1) - big_get_scale(v2));
  // копирование результата
  *result = to_decimal(res);

  return OK;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  set_scale(dst, 0);
  set_sign(dst, src >= 0 ? 0 : 1);
  dst->bits[0] = src >= 0 ? src : -src;
  return OK;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  char str[15];
  int value = 0;
  sprintf(str, "%.6e", src);

  int i = 0;
  for (; i < strlen(str); i++) {
    if (str[i] == 'e') break;
    if (str[i] >= '0' && str[i] <= '9') {
      value *= 10;
      value += str[i] - '0';
    }
  }

  int scale = 0;
  int scale_sign = 0;
  for (; i < strlen(str); i++) {
    if (str[i] == '-') scale_sign = 1;
    if (str[i] >= '0' && str[i] <= '9') {
      scale *= 10;
      scale += str[i] - '0';
    }
  }
  if (scale_sign) scale = -scale;
  scale -= 6;

  s21_big_decimal tmp;
  big_null_decimal(&tmp);
  // tmp положительное, порядок = 0
  tmp.bits[0] = value;
  // знак берем из src
  big_set_sign(&tmp, src >= 0 ? 0 : 1);
  if (scale > 0) {
    // умножаем на 10^scale, порядок = 0
    for (int i = 0; i < scale; i++) {
      mul_10(&tmp);
    }
  } else {
    // копируем порядок src
    big_set_scale(&tmp, -scale);
  }

  // копирование результата
  *dst = to_decimal(tmp);

  return OK;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  s21_decimal ten;
  null_decimal(&ten);
  ten.bits[0] = 10;

  s21_decimal tmp = src;
  for (int i = 0; i < get_scale(src); i++) {
    s21_div(tmp, ten, &tmp);
  }

  set_bit(&tmp, 31, 0);
  int value = tmp.bits[0];
  value = get_sign(tmp) ? -value : value;
  *dst = value;

  return OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) { return 0; }

int s21_floor(s21_decimal value, s21_decimal *result) {
  int scale = get_scale(value);
  int sign = get_sign(value);
  set_sign(&value, 0);
  s21_truncate(value, result);
  if (sign == 1 && s21_is_not_equal(value, *result)) {
    s21_decimal neg_decimal;
    s21_from_int_to_decimal(-1, &neg_decimal);
    s21_add(*result, neg_decimal, result);
  }
  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  s21_decimal half;
  s21_from_int_to_decimal(0.5, &half);
  s21_add(value, half, result);
  s21_truncate(*result, result);
  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int scale = get_scale(value);
  int div = 10 ^ scale;
  s21_decimal div2;
  s21_from_int_to_decimal(div, &div2);
  s21_div(value, div2, result);
  return 0;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  s21_decimal div2;
  s21_from_int_to_decimal(-1, &div2);
  s21_mul(value, div2, result);
  return 0;
}
