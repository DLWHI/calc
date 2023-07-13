#include <check.h>
#include <data_types.h>
#include <stdio.h>

#include "../include/smart_calc.h"

#define EPS 1e-7

double execute_calculation(char *expr, double arg) {
  preprocess(expr);
  list *postfix = to_postfix(expr);
  return calculate(postfix, arg, expr);
}

START_TEST(num_1_plus) {
  char expr[EQ_MAX_LENGTH] = "1+2";
  double answer = 1 + 2;
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 0), EPS);
}
END_TEST

START_TEST(num_1_minus) {
  char expr[EQ_MAX_LENGTH] = "1-2";
  double answer = 1 - 2;
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 0), EPS);
}
END_TEST

START_TEST(num_2_minus) {
  char expr[EQ_MAX_LENGTH] = "sin(1-2)";
  double answer = sin(1 - 2);
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 0), EPS);
}
END_TEST

START_TEST(num_3_minus) {
  char expr[EQ_MAX_LENGTH] = "sin(-2)*4";
  double answer = sin(-2) * 4;
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 0), EPS);
}
END_TEST

START_TEST(num_4_minus) {
  char expr[EQ_MAX_LENGTH] = "3+-4";
  double answer = 3 + -4;
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 0), EPS);
}
END_TEST

START_TEST(num_5_minus) {
  char expr[EQ_MAX_LENGTH] = "3-+4";
  double answer = 3 + -4;
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 0), EPS);
}
END_TEST

START_TEST(num_6_minus) {
  char expr[EQ_MAX_LENGTH] = "3---4";
  double answer = 3 - 4;
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 0), EPS);
}
END_TEST

START_TEST(num_1_combined) {
  char expr[EQ_MAX_LENGTH] = "3*-4";
  double answer = 3 * -4;
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 0), EPS);
}
END_TEST

START_TEST(num_2_combined) {
  char expr[EQ_MAX_LENGTH] = "-3*-4";
  double answer = -3 * -4;
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 0), EPS);
}
END_TEST

START_TEST(num_3_combined) {
  char expr[EQ_MAX_LENGTH] = "exp(3)/x/3.141";
  double answer = exp(3) / 2.0 / 3.141;
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 2), EPS);
}
END_TEST

START_TEST(num_7_minus) {
  char expr[EQ_MAX_LENGTH] = "3-+-4";
  double answer = 3 - +-4;
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 0), EPS);
}
END_TEST

START_TEST(num_2_too_long_string) {
  char expr[258] = {0};
  for (int i = 0; i < 258; i += 2) {
    expr[i] = '2';
    expr[i + 1] = '+';
  }
  expr[257] = '\0';
  ck_assert_float_eq_tol(256, execute_calculation(expr, 0), 1e-7);
}
END_TEST

START_TEST(num_4_error) {
  char expr[EQ_MAX_LENGTH] = "sin(+-+)";
  execute_calculation(expr, 0);
  ck_assert_str_eq(expr, "INVALID EXPRESSION");
}
END_TEST

START_TEST(num_5_error) {
  char expr[EQ_MAX_LENGTH] = "sin(cos())";
  execute_calculation(expr, 0);
  ck_assert_str_eq(expr, "INVALID EXPRESSION");
}
END_TEST

START_TEST(num_6_error) {
  char expr[EQ_MAX_LENGTH] = "^3";
  execute_calculation(expr, 0);
  ck_assert_str_eq(expr, "INVALID EXPRESSION");
}
END_TEST

START_TEST(num_7_trigonometric) {
  char expr[EQ_MAX_LENGTH] = "sin(cos(tan(2+3^6)))";
  double answer = sin(cos(tan(2 + pow(3, 6))));
  ck_assert_float_eq_tol(answer, execute_calculation(expr, 0), EPS);
}
END_TEST

START_TEST(num_7_inf) {
  char expr[EQ_MAX_LENGTH] = "1 / 0";
  ck_assert_float_infinite(execute_calculation(expr, 0));
}
END_TEST

START_TEST(num_8_nan) {
  char expr[EQ_MAX_LENGTH] = "ln(-1)";
  ck_assert_float_nan(execute_calculation(expr, 0));
}
END_TEST

START_TEST(num_9_correct) {
  char expr[EQ_MAX_LENGTH] = "ln(1)";
  ck_assert_float_eq_tol(execute_calculation(expr, 0), 0., EPS);
}
END_TEST

START_TEST(num_10_mod) {
  char expr[EQ_MAX_LENGTH] = "2 mod 3";
  ck_assert_float_eq_tol(execute_calculation(expr, 0), 2., EPS);
}
END_TEST

START_TEST(num_11_mod) {
  char expr[EQ_MAX_LENGTH] = "2.25 mod -3";
  ck_assert_float_eq_tol(execute_calculation(expr, 0), 2.25, EPS);
}
END_TEST

START_TEST(num_12_mod) {
  char expr[EQ_MAX_LENGTH] = "2.25 mod 0.25";
  ck_assert_float_eq_tol(execute_calculation(expr, 0), 0., EPS);
}
END_TEST

START_TEST(num_12_log) {
  char expr[EQ_MAX_LENGTH] = "log(10)";
  ck_assert_float_eq_tol(execute_calculation(expr, 0), 1., EPS);
}
END_TEST

START_TEST(num_13_log) {
  char expr[EQ_MAX_LENGTH] = "log(-1*(sin(5) ^ 2 * cos(2) - 1))";
  double answer = log10(-(pow(sin(5), 2) * cos(2) - 1));
  ck_assert_float_eq_tol(execute_calculation(expr, 0), answer, EPS);
}
END_TEST

START_TEST(num_14_ln) {
  char expr[EQ_MAX_LENGTH] = "ln(2.718281828)";
  double answer = log(2.718281828);
  ck_assert_float_eq_tol(execute_calculation(expr, 0), answer, EPS);
}
END_TEST

START_TEST(num_15_ln) {
  char expr[EQ_MAX_LENGTH] = "ln(125.354^2)";
  double answer = log(pow(125.354, 2));
  ck_assert_float_eq_tol(execute_calculation(expr, 0), answer, EPS);
}
END_TEST

START_TEST(num_16_log) {
  char expr[EQ_MAX_LENGTH] = "log(125.354^2)";
  double answer = log10(pow(125.354, 2));
  ck_assert_float_eq_tol(execute_calculation(expr, 0), answer, EPS);
}
END_TEST

START_TEST(num_17_sin_asin) {
  char expr[EQ_MAX_LENGTH] = "sin(asin(0.5))";
  double answer = sin(asin(0.5));
  ck_assert_float_eq_tol(execute_calculation(expr, 0), answer, EPS);
}
END_TEST

START_TEST(num_18_cos_acos) {
  char expr[EQ_MAX_LENGTH] = "cos(acos(0.5))";
  double answer = sin(asin(0.5));
  ck_assert_float_eq_tol(execute_calculation(expr, 0), answer, EPS);
}
END_TEST

START_TEST(num_19_cos_sin) {
  char expr[EQ_MAX_LENGTH] = "(1+2)*4*cos(x*7-2)+sin(x*2)";
  double answer = (1 + 2) * 4 * cos(2 * 7 - 2) + sin(2 * 2);
  ck_assert_float_eq_tol(execute_calculation(expr, 2.0), answer, EPS);
}
END_TEST

START_TEST(num_20_long_expr) {
  char expr[EQ_MAX_LENGTH] =
      "tan( 3.764) ^sin( 9.445-sin( 7.2889 /  8.0438 -cos( sin(cos( "
      "tan(8.4448))) - 4.482)  )  / tan(cos(cos(sin(cos( "
      "cos(2.003)) )  ) / 0.1315) ))  -  8.8453/ 0.3612";
  double answer = -23.76667454586336;
  ck_assert_float_eq_tol(execute_calculation(expr, 2.0), answer, EPS);
}
END_TEST

START_TEST(num_23_atan_sin) {
  char expr[EQ_MAX_LENGTH] = "atan( 3.764) ^ sin(3)";
  double answer = pow(atan(3.764), sin(3));
  ck_assert_float_eq_tol(execute_calculation(expr, 2.0), answer, EPS);
}
END_TEST

START_TEST(num_23_sqrt) {
  char expr[EQ_MAX_LENGTH] = "sqrt(atan( 3.764) ^ sin(3))";
  double answer = sqrt(pow(atan(3.764), sin(3)));
  ck_assert_float_eq_tol(execute_calculation(expr, 2.0), answer, EPS);
}
END_TEST

START_TEST(num_24_sqrt_error) {
  char expr[EQ_MAX_LENGTH] = "qqrt(atan( 3.764) ^ sin(3))";
  execute_calculation(expr, 0);
  ck_assert_str_eq(expr, "INVALID EXPRESSION");
}
END_TEST

START_TEST(num_25_error) {
  char expr[EQ_MAX_LENGTH] = "2@3";
  execute_calculation(expr, 0);
  ck_assert_str_eq(expr, "INVALID EXPRESSION");
}
END_TEST

START_TEST(num_boobs) {
  char expr[EQ_MAX_LENGTH] = "(.)/(.)";
  execute_calculation(expr, 0);
  ck_assert_str_eq(expr, "INVALID DATA");
}
END_TEST

START_TEST(num_dick) {
  char expr[EQ_MAX_LENGTH] = "./.";
  execute_calculation(expr, 0);
  ck_assert_str_eq(expr, "INVALID DATA");
}
END_TEST

START_TEST(num_bered_boobs) {
  char expr[EQ_MAX_LENGTH] = "( .4)/(.8)";
  double answer = .4/.8;
  ck_assert_float_eq_tol(execute_calculation(expr, 2.0), answer, EPS);
}
END_TEST
START_TEST(num_mod_longer) {
  char expr[EQ_MAX_LENGTH] = "5+5mod2";
  double answer = 5+5%2;
  ck_assert_float_eq_tol(execute_calculation(expr, 2.0), answer, EPS);
}
END_TEST
START_TEST(num_x_and_unary) {
  char expr[EQ_MAX_LENGTH] = "2^x+sinx";
  double answer = 16+sin(4);
  ck_assert_float_eq_tol(execute_calculation(expr, 4.0), answer, EPS);
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, num_1_plus);
  tcase_add_test(tc1_1, num_1_minus);
  tcase_add_test(tc1_1, num_2_minus);
  tcase_add_test(tc1_1, num_3_minus);
  tcase_add_test(tc1_1, num_4_minus);
  tcase_add_test(tc1_1, num_5_minus);
  tcase_add_test(tc1_1, num_6_minus);
  tcase_add_test(tc1_1, num_7_minus);
  tcase_add_test(tc1_1, num_1_combined);
  tcase_add_test(tc1_1, num_2_combined);
  tcase_add_test(tc1_1, num_3_combined);
  tcase_add_test(tc1_1, num_2_too_long_string);
  tcase_add_test(tc1_1, num_4_error);
  tcase_add_test(tc1_1, num_5_error);
  tcase_add_test(tc1_1, num_6_error);
  tcase_add_test(tc1_1, num_7_trigonometric);
  tcase_add_test(tc1_1, num_7_inf);
  tcase_add_test(tc1_1, num_8_nan);
  tcase_add_test(tc1_1, num_9_correct);
  tcase_add_test(tc1_1, num_10_mod);
  tcase_add_test(tc1_1, num_11_mod);
  tcase_add_test(tc1_1, num_12_mod);
  tcase_add_test(tc1_1, num_12_log);
  tcase_add_test(tc1_1, num_13_log);
  tcase_add_test(tc1_1, num_14_ln);
  tcase_add_test(tc1_1, num_15_ln);
  tcase_add_test(tc1_1, num_16_log);
  tcase_add_test(tc1_1, num_17_sin_asin);
  tcase_add_test(tc1_1, num_18_cos_acos);
  tcase_add_test(tc1_1, num_19_cos_sin);
  tcase_add_test(tc1_1, num_20_long_expr);
  tcase_add_test(tc1_1, num_23_atan_sin);
  tcase_add_test(tc1_1, num_23_sqrt);
  tcase_add_test(tc1_1, num_24_sqrt_error);
  tcase_add_test(tc1_1, num_25_error);
  tcase_add_test(tc1_1, num_boobs);
  tcase_add_test(tc1_1, num_dick);
  tcase_add_test(tc1_1, num_bered_boobs);
  tcase_add_test(tc1_1, num_mod_longer);
  tcase_add_test(tc1_1, num_x_and_unary);

  srunner_set_fork_status(sr, CK_NOFORK);
  printf("\033[1;34m");
  srunner_run_all(sr, CK_ENV);
  printf("\033[0;0m");
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);
  return nf == 0 ? 0 : 1;
}
