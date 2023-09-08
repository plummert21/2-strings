#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_string.h"

START_TEST(strncat_str_eq_default) {
  char arr[50] = "Aboba";
  char arr_origin[50] = "Abo";
  s21_size_t len = strlen(arr);

  char* s21_res = s21_strncat(arr, arr_origin, len);
  char* res = strncat(arr, arr_origin, len);

  ck_assert_pstr_eq(s21_res, res);
}
END_TEST

START_TEST(strncat_str_eq_empty) {
  char arr[50] = "Aboba";
  char arr_origin[50] = "";
  s21_size_t len = strlen(arr);

  char* s21_res = s21_strncat(arr, arr_origin, len);
  char* res = strncat(arr, arr_origin, len);

  ck_assert_pstr_eq(s21_res, res);
}
END_TEST

START_TEST(strncat_str_eq_main_empty) {
  char arr[50] = "";
  char arr_origin[50] = "Abo";
  s21_size_t len = strlen(arr_origin);

  char* s21_res = s21_strncat(arr, arr_origin, len);
  char* res = strncat(arr, arr_origin, len);

  ck_assert_pstr_eq(s21_res, res);
}
END_TEST

START_TEST(strncat_str_eq_full_empty) {
  char arr[50] = "";
  char arr_origin[50] = "";
  s21_size_t len = strlen(arr_origin);

  char* s21_res = s21_strncat(arr, arr_origin, len);
  char* res = strncat(arr, arr_origin, len);

  ck_assert_pstr_eq(s21_res, res);
}
END_TEST

Suite* s21_strncat_suite(void) {
  Suite* suite = suite_create("\033[45m-=S21_STRNCAT=-\033[0m");
  TCase* tc_core = tcase_create("s21_tcase_strncat");

  tcase_add_test(tc_core, strncat_str_eq_default);
  tcase_add_test(tc_core, strncat_str_eq_empty);
  tcase_add_test(tc_core, strncat_str_eq_main_empty);
  tcase_add_test(tc_core, strncat_str_eq_full_empty);

  suite_add_tcase(suite, tc_core);

  return suite;
}

int main(void) {
  Suite* suite = s21_strncat_suite();
  SRunner* runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  int failed_count = srunner_ntests_failed(runner);

  srunner_free(runner);

  return failed_count == 0 ? 0 : 1;
}