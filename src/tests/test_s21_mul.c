#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"

// Тест 1
START_TEST(test_mul_1) {
    s21_decimal v1;
    create_decimal(&v1, 0, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 0, 2, 2004, 0, 0);
    s21_decimal result;
    s21_mul(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2),
                   get_scale(v1) + get_scale(v2), v1.bits[0] * v2.bits[0], 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 2
START_TEST(test_mul_2) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 2, 2004, 0, 0);
    s21_decimal result;
    s21_mul(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2),
                   get_scale(v1) + get_scale(v2), v1.bits[0] * v2.bits[0], 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 3
START_TEST(test_mul_3) {
    s21_decimal v1;
    create_decimal(&v1, 0, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 2, 2004, 0, 0);
    s21_decimal result;
    s21_mul(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2),
                   get_scale(v1) + get_scale(v2), v1.bits[0] * v2.bits[0], 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 4
START_TEST(test_mul_4) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 0, 2, 2004, 0, 0);
    s21_decimal result;
    s21_mul(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2),
                   get_scale(v1) + get_scale(v2), v1.bits[0] * v2.bits[0], 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 5. signs (+,+), scales (2,3)
START_TEST(test_mul_5) {
    s21_decimal v1;
    create_decimal(&v1, 0, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 0, 3, 2004, 0, 0);
    s21_decimal result;
    s21_mul(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2),
                   get_scale(v1) + get_scale(v2), v1.bits[0] * v2.bits[0], 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 6. signs (-,-), scales (2,3)
START_TEST(test_mul_6) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 3, 2004, 0, 0);
    s21_decimal result;
    s21_mul(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2),
                   get_scale(v1) + get_scale(v2), v1.bits[0] * v2.bits[0], 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

Suite *mul_suite(void) {
    Suite *s = suite_create("add_tests");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_mul_1);
    tcase_add_test(tc_core, test_mul_2);
    tcase_add_test(tc_core, test_mul_3);
    tcase_add_test(tc_core, test_mul_4);
    tcase_add_test(tc_core, test_mul_5);
    tcase_add_test(tc_core, test_mul_6);

    suite_add_tcase(s, tc_core);
    return s;
}

// Запуск тестов
int main_s21_mul(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = mul_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
