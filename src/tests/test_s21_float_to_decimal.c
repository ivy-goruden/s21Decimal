#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"

// Тест 1
START_TEST(test_float_to_decimal_1) {
    float value = 1.5;
    s21_decimal result;
    s21_from_float_to_decimal(value, &result);
    s21_decimal orig;
    create_decimal(&orig, 0, 6, 1500000, 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 2
START_TEST(test_float_to_decimal_2) {
    float value = -1.5;
    s21_decimal result;
    s21_from_float_to_decimal(value, &result);
    s21_decimal orig;
    create_decimal(&orig, 1, 6, 1500000, 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 3
START_TEST(test_float_to_decimal_3) {
    float value = 6.6743e-11;
    s21_decimal result;
    s21_from_float_to_decimal(value, &result);
    s21_decimal orig;
    create_decimal(&orig, 0, 17, 6674300, 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 4
START_TEST(test_float_to_decimal_4) {
    float value = 6.6e3;
    s21_decimal result;
    s21_from_float_to_decimal(value, &result);
    s21_decimal orig;
    create_decimal(&orig, 0, 3, 6600000, 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 5
START_TEST(test_float_to_decimal_5) {
    float value = 6.6e8;
    s21_decimal result;
    s21_from_float_to_decimal(value, &result);
    s21_decimal orig;
    create_decimal(&orig, 0, 0, 660000000, 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

Suite *float_to_decimal_suite(void) {
    Suite *s = suite_create("zero_tests");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_float_to_decimal_1);
    tcase_add_test(tc_core, test_float_to_decimal_2);
    tcase_add_test(tc_core, test_float_to_decimal_3);
    tcase_add_test(tc_core, test_float_to_decimal_4);
    tcase_add_test(tc_core, test_float_to_decimal_5);

    suite_add_tcase(s, tc_core);
    return s;
}

// Запуск тестов
int main_s21_float_to_decimal(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = float_to_decimal_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
