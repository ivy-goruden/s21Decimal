#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"

// Тест 1
START_TEST(test_decimal_to_int_1) {
    int result;
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1804, 0, 0);
    s21_from_decimal_to_int(v1, &result);

    int value = -18;
    ck_assert_int_eq(result, value);
}
END_TEST

// Тест 2
START_TEST(test_decimal_to_int_2) {
    int result;
    s21_decimal v1;
    create_decimal(&v1, 0, 0, 1804044, 0, 0);
    s21_from_decimal_to_int(v1, &result);

    int value = 1804044;
    ck_assert_int_eq(result, value);
}
END_TEST

// Тест 3
START_TEST(test_decimal_to_int_3) {
    int result;
    s21_decimal v1;
    create_decimal(&v1, 0, 0, 0, 0, 0);
    s21_from_decimal_to_int(v1, &result);

    int value = 0;
    ck_assert_int_eq(result, value);
}
END_TEST

// Тест 4
START_TEST(test_decimal_to_int_4) {
    int result;
    s21_decimal v1;
    create_decimal(&v1, 0, 5, 808080807, 0, 0);
    s21_from_decimal_to_int(v1, &result);

    int value = 8080;
    ck_assert_int_eq(result, value);
}
END_TEST

Suite *decimal_to_int_suite(void) {
    Suite *s = suite_create("zero_tests");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_decimal_to_int_1);
    tcase_add_test(tc_core, test_decimal_to_int_2);
    tcase_add_test(tc_core, test_decimal_to_int_3);
    tcase_add_test(tc_core, test_decimal_to_int_4);

    suite_add_tcase(s, tc_core);
    return s;
}

// Запуск тестов
int main_s21_decimal_to_int(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = decimal_to_int_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
