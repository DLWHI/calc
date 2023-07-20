#include <gtest/gtest.h>
#include "../src/model/Tokenizer.h"

std::string to_string(s21::list<std::string> tokens) {
  std::string result;
  for (auto token: tokens)
    result += token;
  return result;
}



TEST(TokenizerTest, case_missed_multp_1) {
  s21::Tokenizer tr;
  std::string expected = "sin(x)*cos(x)";
  s21::list<std::string> returned = tr.Tokenize("sin(x)cos(x)");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_missed_multp_2) {
  s21::Tokenizer tr;
  std::string expected = "2*x";
  s21::list<std::string> returned = tr.Tokenize("2x");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_missed_multp_3) {
  s21::Tokenizer tr;
  std::string expected = "x*2";
  s21::list<std::string> returned = tr.Tokenize("x2");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_missed_multp_4) {
  s21::Tokenizer tr;
  std::string expected = "sin(37.2)*x";
  s21::list<std::string> returned = tr.Tokenize("sin(37.2)x");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_missed_multp_5) {
  s21::Tokenizer tr;
  std::string expected = "sin(x)*.24";
  s21::list<std::string> returned = tr.Tokenize("sin(x).24");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_missed_multp_6) {
  s21::Tokenizer tr;
  std::string expected = "x*x";
  s21::list<std::string> returned = tr.Tokenize("xx");
  EXPECT_EQ(expected, to_string(returned));
}



TEST(TokenizerTest, case_unopened_bracket_1) {
  s21::Tokenizer tr;
  std::string expected = "x*cos(x)";
  s21::list<std::string> returned = tr.Tokenize("x)cosx");

  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_unopened_bracket_2) {
  s21::Tokenizer tr;
  std::string expected = "cos(7.3)";
  s21::list<std::string> returned = tr.Tokenize(")cos(7.3)");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_unopened_bracket_3) {
  s21::Tokenizer tr;
  std::string expected = "x*cos(x)";
  s21::list<std::string> returned = tr.Tokenize("x)cosx");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_unopened_bracket_4) {
  s21::Tokenizer tr;
  std::string expected = "(sin(0)*x)*0*00*0";
  s21::list<std::string> returned = tr.Tokenize("(sin(0)x)))0))00)0");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_unopened_bracket_5) {
  s21::Tokenizer tr;
  std::string expected = "0*00*0*(sin(0)*x)";
  s21::list<std::string> returned = tr.Tokenize(")))0))00)0(sin(0)x");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_unopened_bracket_6) {
  s21::Tokenizer tr;
  std::string expected = "x";
  s21::list<std::string> returned = tr.Tokenize(")x");
  EXPECT_EQ(expected, to_string(returned));
}




TEST(TokenizerTest, case_add_brackets_1) {
  s21::Tokenizer tr;
  std::string expected = "sin(2*x)";
  s21::list<std::string> returned = tr.Tokenize("sin2x");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_add_brackets_2) {
  s21::Tokenizer tr;
  std::string expected = "sin(x)*cos(.261)";
  s21::list<std::string> returned = tr.Tokenize("sinxcos.261");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_add_brackets_3) {
  s21::Tokenizer tr;
  std::string expected = "sin(cos(x))";
  s21::list<std::string> returned = tr.Tokenize("sincosx");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_add_brackets_4) {
  s21::Tokenizer tr;
  std::string expected = "sin(2*x*x)*cos(5.5*x)";
  s21::list<std::string> returned = tr.Tokenize("sin2xxcos5.5x");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_add_brackets_5) {
  s21::Tokenizer tr;
  std::string expected = "sin(2)";
  s21::list<std::string> returned = tr.Tokenize("sin2");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_add_brackets_6) {
  s21::Tokenizer tr;
  std::string expected = "sin(2)";
  s21::list<std::string> returned = tr.Tokenize("sin(2");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_add_brackets_7) {
  s21::Tokenizer tr;
  std::string expected = "1+sin(~2)";
  s21::list<std::string> returned = tr.Tokenize("1+sin-2");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_add_brackets_8) {
  s21::Tokenizer tr;
  std::string expected = "sin(~(2+x*x-sin(0)))";
  s21::list<std::string> returned = tr.Tokenize("sin-(2+xx-sin(0))");
  EXPECT_EQ(expected, to_string(returned));
}



TEST(TokenizerTest, case_operators_1) {
  s21::Tokenizer tr;
  std::string expected = "x*~2";
  s21::list<std::string> returned = tr.Tokenize("x*-2");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_2) {
  s21::Tokenizer tr;
  std::string expected = "3+~4";
  s21::list<std::string> returned = tr.Tokenize("3+-4");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_3) {
  s21::Tokenizer tr;
  std::string expected = "3-#4";
  s21::list<std::string> returned = tr.Tokenize("3-+4");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_4) {
  s21::Tokenizer tr;
  std::string expected = "3-~~4";
  s21::list<std::string> returned = tr.Tokenize("3---4");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_5) {
  s21::Tokenizer tr;
  std::string expected = "~3*~4";
  s21::list<std::string> returned = tr.Tokenize("-3*-4");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_6) {
  s21::Tokenizer tr;
  std::string expected = "2%3";
  s21::list<std::string> returned = tr.Tokenize("2%3");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_7) {
  s21::Tokenizer tr;
  std::string expected = "2%3";
  s21::list<std::string> returned = tr.Tokenize(" 2  %    3");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_8) {
  s21::Tokenizer tr;
  std::string expected = "x^x";
  s21::list<std::string> returned = tr.Tokenize("x^x");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_9) {
  s21::Tokenizer tr;
  std::string expected = "8^~2";
  s21::list<std::string> returned = tr.Tokenize("8^-2");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_10) {
  s21::Tokenizer tr;
  std::string expected = "3%2";
  s21::list<std::string> returned = tr.Tokenize("3%2");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_11) {
  s21::Tokenizer tr;
  std::string expected = "3%~.32";
  s21::list<std::string> returned = tr.Tokenize("3%-.32");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_12) {
  s21::Tokenizer tr;
  std::string expected = "exp(3)/x/3.141";
  s21::list<std::string> returned = tr.Tokenize("exp(3)/x/3.141");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_13) {
  s21::Tokenizer tr;
  std::string expected = "3-#~4";
  s21::list<std::string> returned = tr.Tokenize("3-+-4");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_operators_14) {
  s21::Tokenizer tr;
  std::string expected = "tan(3.764)^sin(9.445)";
  s21::list<std::string> returned = tr.Tokenize("tan3.764^sin9.445");
  EXPECT_EQ(expected, to_string(returned));
}



TEST(TokenizerTest, case_scientific_1) {
  s21::Tokenizer tr;
  std::string expected = "3e8";
  s21::list<std::string> returned = tr.Tokenize("3e8");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_scientific_2) {
  s21::Tokenizer tr;
  std::string expected = "1.6e-19";
  s21::list<std::string> returned = tr.Tokenize("1.6e-19");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_scientific_3) {
  s21::Tokenizer tr;
  std::string expected = "9.1e-31*1.6e-19";
  s21::list<std::string> returned = tr.Tokenize("9.1e-31*1.6e-19");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_scientific_4) {
  s21::Tokenizer tr;
  std::string expected = "exp(~9.1e-31*x*x/(2*1.38e-23*10e6))";
  s21::list<std::string> returned = tr.Tokenize("exp(-9.1e-31*xx/(2*1.38e-23*10e6))");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_scientific_5) {
  s21::Tokenizer tr;
  std::string expected = "sin(3.14e-0)";
  s21::list<std::string> returned = tr.Tokenize("sin(3.14e-0)");
  EXPECT_EQ(expected, to_string(returned));
}



TEST(TokenizerTest, case_other_1) {
  s21::Tokenizer tr;
  std::string expected = "22";
  s21::list<std::string> returned = tr.Tokenize("22");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_2) {
  s21::Tokenizer tr;
  std::string expected = "sin(2*x*x*(1+x))*cos(2*x)*(1-x)";
  s21::list<std::string> returned = tr.Tokenize("sin(2xx(1+x))cos2x(1-x)");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_3) {
  s21::Tokenizer tr;
  std::string expected = "(1+x*sin(x))*sin(cos(1+x-cos(x)))";
  s21::list<std::string> returned = tr.Tokenize("(1+xsinx)sincos(1+x-cosx)");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_4) {
  s21::Tokenizer tr;
  std::string expected = "(1+x*sin(x))*sin(cos(1+x-cos(x)))";
  s21::list<std::string> returned = tr.Tokenize("(1+x*sin(x))*sin(cos(1+x-cos(x)))");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_5) {
  s21::Tokenizer tr;
  std::string expected = "cos(x-1)*x*cos(x)-sin(x)*sin(1-x*x)";
  s21::list<std::string> returned = tr.Tokenize("cos(x-1)xcosx-sinxsin(1-xx)");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_6) {
  s21::Tokenizer tr;
  std::string expected = "cos(~x)*cos(x)-sin(x)*cos(sqrt(sin(1-x*x+sqrt(1+cos(sin(2*x))))))";
  s21::list<std::string> returned = tr.Tokenize("cos-xcosx-sinxcossqrtsin(1-xx+sqrt(1+cossin2x))");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_7) {
  s21::Tokenizer tr;
  std::string expected = "cos(~x)*cos(x)-sin(x)*cos(sqrt(sin(1-x*x+sqrt(1+cos(sin(2*x))))))";
  s21::list<std::string> returned = tr.Tokenize("cos-xcosx-sinxcossqrtsin(1-xx+sqrt(1+cossin2x))");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_8) {
  s21::Tokenizer tr;
  std::string expected = "cos(x)-1*x*cos(x)-sin(sin(x))*sin(1-x*x)";
  s21::list<std::string> returned = tr.Tokenize("cosx-1xcosx-sinsinxsin(1-xx)");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_9) {
  s21::Tokenizer tr;
  std::string expected = "((cos(x-tg(cos(sin(sin(x*sin(1-x*x)))))+x*cos(x))))";
  s21::list<std::string> returned = tr.Tokenize("((cos(x-tgcossinsin(xsin(1-xx))+xcosx");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_10) {
  s21::Tokenizer tr;
  std::string expected = "cos(x)-tg(cos(sin(sin(x*sin(1-x*x)))))+x*cos(x)";
  s21::list<std::string> returned = tr.Tokenize("cosx-tgcossinsin(xsin(1-xx))+xcosx");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_11) {
  s21::Tokenizer tr;
  std::string expected = "cos(~x)";
  s21::list<std::string> returned = tr.Tokenize("cos(~x)");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_12) {
  s21::Tokenizer tr;
  std::string expected = "sin(~(2+x*x-sin(0)))";
  s21::list<std::string> returned = tr.Tokenize("sin~(2+xx-sin(0))");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_13) {
  s21::Tokenizer tr;
  std::string expected = "sin(~(2-x*x-sin(0)))";
  s21::list<std::string> returned = tr.Tokenize("sin~(2~xx-sin(0))");
  EXPECT_EQ(expected, to_string(returned));
}

TEST(TokenizerTest, case_other_14) {
  s21::Tokenizer tr;
  std::string expected = "sin(~(2-x*x-sin(0)))";
  s21::list<std::string> returned = tr.Tokenize("  sin  ~(2~xx     -sin  (0 ))    ");
  EXPECT_EQ(expected, to_string(returned));
}



TEST(TokenizerTest, case_error_unfinished_1) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("++"), std::logic_error);
}

TEST(TokenizerTest, case_error_unfinished_2) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("2("), std::logic_error);
}

TEST(TokenizerTest, case_error_unfinished_3) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize(")("), std::logic_error);
}

TEST(TokenizerTest, case_error_unfinished_4) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("sin(x)("), std::logic_error);
}

TEST(TokenizerTest, case_error_unfinished_5) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("sin+"), std::logic_error);
}

TEST(TokenizerTest, case_error_unfinished_6) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("2sin"), std::logic_error);
}

TEST(TokenizerTest, case_error_unfinished_7) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("xsin"), std::logic_error);
}

TEST(TokenizerTest, case_error_unfinished_8) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("sincos"), std::logic_error);
}



TEST(TokenizerTest, case_error_mismatch_op_1) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("(*1+3"), std::logic_error);
}

TEST(TokenizerTest, case_error_mismatch_op_2) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("1+sin*2"), std::logic_error);
}

TEST(TokenizerTest, case_error_mismatch_op_3) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("cos-xcosx-sinxcos*sqrtsin-(1-xx+sqrt(1+cossin2x))"), std::logic_error);
}

TEST(TokenizerTest, case_error_mismatch_op_4) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("3-mod-.32"), std::logic_error);
}

TEST(TokenizerTest, case_error_mismatch_op_5) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("sin(+-+)"), std::logic_error);
}



TEST(TokenizerTest, case_error_brackets_broken_1) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("(sin)x"), std::logic_error);
}

TEST(TokenizerTest, case_error_brackets_broken_2) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("(1+)x"), std::logic_error);
}

TEST(TokenizerTest, case_error_brackets_broken_3) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("()"), std::logic_error);
}

TEST(TokenizerTest, case_error_brackets_broken_4) {
  s21::Tokenizer tr;
  EXPECT_THROW(tr.Tokenize("((cos(x-tgcos)sinsin(xsin(1-xx))+xcosx"), std::logic_error);
}


// caught at calculation
// TEST(TokenizerTest, case_broken_scientific_1) {
//   s21::Tokenizer tr;
//   EXPECT_THROW(tr.Tokenize("2e"), std::logic_error);
// }

// TEST(TokenizerTest, case_broken_scientific_2) {
//   s21::Tokenizer tr;
//   EXPECT_THROW(tr.Tokenize("2.2e+sinx"), std::logic_error);
// }

// TEST(TokenizerTest, case_broken_scientific_3) {
//   s21::Tokenizer tr;
//   std::cout << to_string(tr.Tokenize("2.2esinx")) << std::endl;
//   EXPECT_THROW(tr.Tokenize("2.2esinx"), std::logic_error);
// }

// TEST(TokenizerTest, case_broken_scientific_4) {
//   s21::Tokenizer tr;
//   EXPECT_THROW(tr.Tokenize("2.2e-xx"), std::logic_error);
// }

// TEST(TokenizerTest, case_broken_scientific_5) {
//   s21::Tokenizer tr;
//   EXPECT_THROW(tr.Tokenize("2.2e*10"), std::logic_error);
// }

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

