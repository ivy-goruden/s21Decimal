#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "main_test.h"

// Тест 0
START_TEST(test_add_0) {
    s21_decimal v1;
    from_str_to_decimal("7.9228162514264337593543950335", &v1);
    s21_decimal v2;
    from_str_to_decimal("7.9228162514264337593543950335", &v2);
    s21_decimal act;
    s21_add(v1, v2, &act);
    s21_decimal exp;
    from_str_to_decimal("15.845632502852867518708790067", &exp);

    print_decimal(act);
    printf("\n");
    print_decimal(exp);
    printf("\n");

    int eq = s21_is_equal(act, exp);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 1
START_TEST(test_add_1) {
    s21_decimal v1;
    create_decimal(&v1, 0, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 0, 2, 2004, 0, 0);
    s21_decimal result;
    s21_add(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, 0, 2, v2.bits[0] + v1.bits[0], 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 2
START_TEST(test_add_2) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 2, 2004, 0, 0);
    s21_decimal result;
    s21_add(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, 1, 2, v2.bits[0] + v1.bits[0], 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 3
START_TEST(test_add_3) {
    s21_decimal v1;
    create_decimal(&v1, 0, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 2, 2004, 0, 0);
    s21_decimal result;
    s21_add(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, 1, 2, v2.bits[0] - v1.bits[0], 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 4
START_TEST(test_add_4) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 0, 2, 2004, 0, 0);
    s21_decimal result;
    s21_add(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, 0, 2, v2.bits[0] - v1.bits[0], 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 5. signs (+,+), scales (2,3)
START_TEST(test_add_5) {
    s21_decimal v1;
    create_decimal(&v1, 0, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 0, 3, 2004, 0, 0);
    s21_decimal result;
    s21_add(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, 0, 3, v2.bits[0] + v1.bits[0] * 10, 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

// Тест 6. signs (-,-), scales (2,3)
START_TEST(test_add_6) {
    s21_decimal v1;
    create_decimal(&v1, 1, 2, 1804, 0, 0);
    s21_decimal v2;
    create_decimal(&v2, 1, 3, 2004, 0, 0);
    s21_decimal result;
    s21_add(v1, v2, &result);

    s21_decimal orig;
    create_decimal(&orig, 1, 3, v2.bits[0] + v1.bits[0] * 10, 0, 0);

    int eq = s21_is_equal(result, orig);
    ck_assert_int_eq(eq, 1);
}
END_TEST

Suite *add_suite(void) {
    Suite *s = suite_create("add_tests");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_add_0);
    // tcase_add_test(tc_core, test_add_1);
    // tcase_add_test(tc_core, test_add_2);
    // tcase_add_test(tc_core, test_add_3);
    // tcase_add_test(tc_core, test_add_4);
    // tcase_add_test(tc_core, test_add_5);
    // tcase_add_test(tc_core, test_add_6);

    suite_add_tcase(s, tc_core);
    return s;
}

// Запуск тестов
int main_s21_add(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = add_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
