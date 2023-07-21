#include <gtest/gtest.h>

#include "../src/model/DefaultModel.h"

class ModelIntegrationTest : public ::testing::Test {
  protected:
    void SetUp() { 
      subject = new s21::DefaultModel;
    }
    void TearDown() { 
      delete subject;
    }
    s21::ICalculationModel *subject;
    double eps = 1e-7;
};

TEST_F(ModelIntegrationTest, case_trivial_1) {
  subject->setExpression("1+2");
  constexpr double expected = 1 + 2;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_2) {
  subject->setExpression("1-2");
  constexpr double expected = 1 - 2;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_3) {
  subject->setExpression("sin(1-2)");
  double expected = sin(1-2);
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_4) {
  subject->setExpression("sin(-2)*4");
  double expected = sin(-2) * 4;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_5) {
  subject->setExpression("1 / 0");
  double expected = INFINITY;
  EXPECT_EQ(subject->Calculate(), expected);
}

TEST_F(ModelIntegrationTest, case_trivial_6) {
  subject->setExpression("ln(-1)");
  double expected = NAN;
  EXPECT_NE(subject->Calculate(), expected);
}

TEST_F(ModelIntegrationTest, case_trivial_7) {
  subject->setExpression("log(1.25354e2^2)");
  double expected = log10(pow(1.25354e2, 2));
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_8) {
  subject->setExpression("sin(asin(0.5))");
  double expected = sin(asin(0.5));
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_9) {
  subject->setExpression("( .4)/(.8)");
  double expected = 0.5;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}


TEST_F(ModelIntegrationTest, case_unary_1) {
  subject->setExpression("3+-4");
  constexpr double expected = 3+-4;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_unary_2) {
  subject->setExpression("3-+4");
  constexpr double expected = 3-+4;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_unary_3) {
  subject->setExpression("3---4");
  constexpr double expected = -1;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_unary_4) {
  subject->setExpression("3*-4");
  constexpr double expected = 3*-4;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_unary_5) {
  subject->setExpression("3-+-4");
  constexpr double expected = 3-+-4;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}



TEST_F(ModelIntegrationTest, case_combined_functions_1) {
  subject->setExpression("exp(3)/x/3.141");
  double expected = exp(3)/2.0/3.141;
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_2) {
  subject->setExpression("sin(cos(tan(2+3^6)))");
  double expected = sin(cos(tan(2 + pow(3, 6))));
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_3) {
  subject->setExpression("2 mod 3");
  constexpr double expected = 2;
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_4) {
  subject->setExpression("2.25 mod -3");
  constexpr double expected = 2.25;
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_5) {
  subject->setExpression("2.25 mod 0.25");
  constexpr double expected = 0;
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_6) {
  subject->setExpression("log(-1*(sin(5) ^ 2 * cos(2) - 1))");
  double expected = log10(-(pow(sin(5), 2) * cos(2) - 1));
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_7) {
  subject->setExpression("4(1+2)cos(x*7-2)+sin2x");
  double expected = (1 + 2) * 4 * cos(2 * 7 - 2) + sin(2 * 2);
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_8) {
  subject->setExpression("tan3.764^sin9.445-sin7.2889/8.0438 -cos sin cos"
      "tan(8.4448 - 4.482)   / tan(cos(cos(sin(cos( "
      "cos(2.003)) )  ) / 0.1315) ))  -  8.8453/ 0.3612");
  constexpr double expected = -24.926337090;
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_9) {
  subject->setExpression("sqrt(atan3.764^ sin3x)");
  constexpr double expected = 1.144653;
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_10) {
  subject->setExpression("5+5mod2");
  constexpr double expected = 5+5%2;
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_11) {
  subject->setExpression("2^x+sinx");
  double expected = 16+sin(4);
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

// START_TEST(num_24_sqrt_error) {
//   char expr[EQ_MAX_LENGTH] = "qqrt(atan( 3.764) ^ sin(3))";
//   execute_calculation(expr, 0);
//   ck_assert_str_eq(expr, "INVALID EXPRESSION");
// }
// END_TEST

// START_TEST(num_25_error) {
//   char expr[EQ_MAX_LENGTH] = "2@3";
//   execute_calculation(expr, 0);
//   ck_assert_str_eq(expr, "INVALID EXPRESSION");
// }
// END_TEST

// START_TEST(num_boobs) {
//   char expr[EQ_MAX_LENGTH] = "(.)/(.)";
//   execute_calculation(expr, 0);
//   ck_assert_str_eq(expr, "INVALID DATA");
// }
// END_TEST

// START_TEST(num_dick) {
//   char expr[EQ_MAX_LENGTH] = "./.";
//   execute_calculation(expr, 0);
//   ck_assert_str_eq(expr, "INVALID DATA");
// }
// END_TEST


// START_TEST(num_2_too_long_string) {
//   char expr[258] = {0};
//   for (int i = 0; i < 258; i += 2) {
//     expr[i] = '2';
//     expr[i + 1] = '+';
//   }
//   expr[257] = '\0';
//   ck_assert_float_eq_tol(256, execute_calculation(expr, 0), 1e-7);
// }
// END_TEST

// START_TEST(num_4_error) {
//   char expr[EQ_MAX_LENGTH] = "sin(+-+)";
//   execute_calculation(expr, 0);
//   ck_assert_str_eq(expr, "INVALID EXPRESSION");
// }
// END_TEST

// START_TEST(num_5_error) {
//   char expr[EQ_MAX_LENGTH] = "sin(cos())";
//   execute_calculation(expr, 0);
//   ck_assert_str_eq(expr, "INVALID EXPRESSION");
// }
// END_TEST

// START_TEST(num_6_error) {
//   char expr[EQ_MAX_LENGTH] = "^3";
//   execute_calculation(expr, 0);
//   ck_assert_str_eq(expr, "INVALID EXPRESSION");
// }
// END_TEST
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
