#include <gtest/gtest.h>

#include "../src/model/DefaultModel.h"

typedef s21::DefaultModel TestingModel;

class ModelIntegrationTest : public ::testing::Test {
  protected:
    void SetUp() { 
      subject = new TestingModel;
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

TEST_F(ModelIntegrationTest, case_trivial_10) {
  subject->setExpression("2 mod 3");
  constexpr double expected = 2;
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_11) {
  subject->setExpression("2.25 mod -3");
  constexpr double expected = 2.25;
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_12) {
  subject->setExpression("2.25 mod 0.25");
  constexpr double expected = 0;
  EXPECT_NEAR(subject->Calculate(2), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_13) {
  subject->setExpression("3e8");
  constexpr double expected = 3e8;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_14) {
  subject->setExpression("1.6e-19");
  constexpr double expected = 1.6e-19;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_15) {
  subject->setExpression("9.1e-31*1.6e-19");
  constexpr double expected = 9.1e-31*1.6e-19;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_trivial_16) {
  subject->setExpression("sin(3.14e-0)");
  double expected = sin(3.14e-0);
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
  constexpr double expected = 1;
  EXPECT_NEAR(subject->Calculate(1.0471975511965976), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_10) {
  subject->setExpression("5+5mod2");
  constexpr double expected = 5+5%2;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_11) {
  subject->setExpression("2^x+sinx");
  double expected = 16+sin(4);
  EXPECT_NEAR(subject->Calculate(4), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_12) {
  subject->setExpression("2^3^2");
  constexpr double expected = 512;
  EXPECT_NEAR(subject->Calculate(), expected, eps);
}

TEST_F(ModelIntegrationTest, case_combined_functions_13) {
  subject->setExpression("exp(~9.1e-31*x*x/(2*1.38e-23*10e6))");
  constexpr double expected = 0.999999999999975638;
  EXPECT_NEAR(subject->Calculate(M_E), expected, eps);
}



TEST_F(ModelIntegrationTest, case_broken_numbers_1) {
  subject->setExpression("2.33.4cos(x0)");
  EXPECT_THROW(subject->Calculate(), std::invalid_argument);
}

TEST_F(ModelIntegrationTest, case_broken_numbers_2) {
  subject->setExpression("2e");
  EXPECT_THROW(subject->Calculate(), std::invalid_argument);
}

TEST_F(ModelIntegrationTest, case_broken_numbers_3) {
  subject->setExpression("2.2e+sinx");
  EXPECT_THROW(subject->Calculate(), std::invalid_argument);
}

TEST_F(ModelIntegrationTest, case_broken_numbers_4) {
  subject->setExpression("2.2esinx");
  EXPECT_THROW(subject->Calculate(), std::invalid_argument);
}

TEST_F(ModelIntegrationTest, case_broken_numbers_5) {
  subject->setExpression("2.2e.88-xx");
  EXPECT_THROW(subject->Calculate(), std::invalid_argument);
}

TEST_F(ModelIntegrationTest, case_broken_numbers_6) {
  subject->setExpression("2.2e*10");
  EXPECT_THROW(subject->Calculate(), std::invalid_argument);
}



TEST_F(ModelIntegrationTest, case_boobs) {
  subject->setExpression("(.)/(.)");
  EXPECT_THROW(subject->Calculate(), std::invalid_argument);
}

TEST_F(ModelIntegrationTest, case_dick) {
  subject->setExpression("./.");
  EXPECT_THROW(subject->Calculate(), std::invalid_argument);
}

TEST_F(ModelIntegrationTest, case_long) {
  std::string longd = "7";
  for (int i = 0; i < 255; longd += "+2", i++) { };
  EXPECT_THROW(subject->setExpression(longd), std::invalid_argument);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
