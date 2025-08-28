#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "main_test.h"

// Тест 1
START_TEST(test_div_1) {
    s21_decimal v1;
    create_decimal(&v1, 0, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 0, 2, 200, 0, 0);
    s21_decimal result;
    s21_div(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2),
                   get_scale(v1) - get_scale(v2), v1.bits[0] / v2.bits[0], 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 2
START_TEST(test_div_2) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 2, 200, 0, 0);
    s21_decimal result;
    s21_div(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2),
                   get_scale(v1) - get_scale(v2), v1.bits[0] / v2.bits[0], 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 3
START_TEST(test_div_3) {
    s21_decimal v1;
    create_decimal(&v1, 0, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 2, 200, 0, 0);
    s21_decimal result;
    s21_div(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2),
                   get_scale(v1) - get_scale(v2), v1.bits[0] / v2.bits[0], 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 4
START_TEST(test_div_4) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 0, 2, 200, 0, 0);
    s21_decimal result;
    s21_div(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2),
                   get_scale(v1) - get_scale(v2), v1.bits[0] / v2.bits[0], 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 5. signs (+,+), scales (2,3)
START_TEST(test_div_5) {
    s21_decimal v1;
    create_decimal(&v1, 0, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 0, 3, 200, 0, 0);
    s21_decimal result;
    s21_div(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2), 0,
                   v1.bits[0] * 10 / v2.bits[0], 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 6. signs (-,-), scales (2,3)
START_TEST(test_div_6) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 3, 200, 0, 0);
    s21_decimal result;
    s21_div(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2), 0,
                   v1.bits[0] * 10 / v2.bits[0], 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 7. signs (-,-), scales (2,3)
START_TEST(test_div_7) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 3, 1, 0, 0);
    s21_decimal result;
    s21_div(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2), 0, v1.bits[0] * 10, 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 8. signs (-,-), scales (2,3)
START_TEST(test_div_8) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 18, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 3, 1000, 0, 0);
    s21_decimal result;
    s21_div(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2), 0, 0, 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 9. division by zero
START_TEST(test_div_9) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1000, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 3, 0, 0, 0);
    s21_decimal result;
    int error = s21_div(v1, v2, &result);

    ck_assert_int_eq(error, 1);
}
END_TEST

// Тест 10. signs (-,-), scales (3,2)
START_TEST(test_div_10) {
    s21_decimal v1;
    create_decimal(&v1, 1, 3, 18000, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 2, 10000, 0, 0);
    s21_decimal result;
    s21_div(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, get_sign(v1) ^ get_sign(v2),
                   get_scale(v1) - get_scale(v2), v1.bits[0] / v2.bits[0], 0,
                   0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

Suite *div_suite(void) {
    Suite *s = suite_create("add_tests");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_div_1);
    tcase_add_test(tc_core, test_div_2);
    tcase_add_test(tc_core, test_div_3);
    tcase_add_test(tc_core, test_div_4);
    tcase_add_test(tc_core, test_div_5);
    tcase_add_test(tc_core, test_div_6);
    tcase_add_test(tc_core, test_div_7);
    tcase_add_test(tc_core, test_div_8);
    tcase_add_test(tc_core, test_div_9);
    tcase_add_test(tc_core, test_div_10);

    suite_add_tcase(s, tc_core);
    return s;
}

// Запуск тестов
int main_s21_div(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = div_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
