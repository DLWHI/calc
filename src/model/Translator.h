#ifndef SRC_MODEL_TRANSLATOR_H_
#define SRC_MODEL_TRANSLATOR_H_
#include "ITranslationModel.h"

namespace s21 {
class bad_expression final : public std::exception {
  public:
    bad_expression(const std::string& what_arg) : msg_(what_arg){};

    const char* what() const noexcept { return msg_.c_str();}
  private:
    std::string msg_;
};

class Translator : public ITranslationModel  {
  public:
    static constexpr size_t kMaxExprLength = 255;

    vector<std::string> translate(const std::string& expression) const;

    void tokenize(const std::string& expression) const;

    std::string fix(const std::string& expression) const;
  private:
    typedef std::string::const_iterator position;

    static constexpr std::string_view kOperators = "+-/*^%~#";

    enum class TokenType {
      DIGIT,
      OPERATOR,
      FUNCTION,
      OPEN_BRACKET,
      CLOSE_BRACKET,
      ARG,
      UNKNOWN
    };

    void PushBrackets(int count, std::string& dest) const noexcept;
    void PushToken(position& pos, std::string& dest) const noexcept;

    TokenType GetTokenType(const position& pos) const noexcept;

    constexpr bool skipsMultiplyRhs(TokenType token) const;
    constexpr bool skipsMultiplyLhs(TokenType token) const;
    constexpr bool skipsFunctionBracket(TokenType token) const;
    constexpr bool isNumeric(TokenType token) const;
    constexpr bool isOneSymboled(TokenType token) const;
    constexpr bool finishesExpr(TokenType token) const;
};
}

#endif  // SRC_MODEL_TRANSLATOR_H_
