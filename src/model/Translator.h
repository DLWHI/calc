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

    vector<std::string> Translate(const std::string& expression) const;

    void Tokenize(const std::string& expression) const;

    std::string Fix(const std::string& expression);
  private:
    typedef std::string::const_iterator position;

    static constexpr std::string_view kOperators = "+-/*^%~#";

    enum class TokenType {
      DIGIT,
      ARG,
      OPERATOR,
      FUNCTION,
      OPEN_BRACKET,
      CLOSE_BRACKET,
      UNKNOWN
    };

    struct TranslatorState
    {
      std::string fixed;
      TokenType prev;
      TokenType current;
      int unclosed;
      int bracket;
    };
    
    void TryOpenBracket(std::string& dest)  noexcept;
    void ManageBrackets();

    void PushBrackets(int count, std::string& dest) noexcept;
    void PushToken(std::string& dest) noexcept;

    TokenType GetTokenType(const position& pos) const noexcept;

    constexpr bool skipsMultiplyRhs(TokenType token) const noexcept;
    constexpr bool skipsMultiplyLhs(TokenType token) const noexcept;
    constexpr bool isNumeric(TokenType token) const noexcept;
    constexpr bool isOneSymboled(TokenType token) const noexcept;
    constexpr bool finishesExpr(TokenType token) const noexcept;

    TokenType prev_token;
    TokenType current_token;
    int unclosed;
    int bracket;
    position pos;
    position end;

};
}

#endif  // SRC_MODEL_TRANSLATOR_H_
