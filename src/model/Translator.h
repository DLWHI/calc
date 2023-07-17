#ifndef SRC_MODEL_TRANSLATOR_H_
#define SRC_MODEL_TRANSLATOR_H_

#include "ITranslationModel.h"
#include "../containers/array.h"

// TODO:
// - Add empty brackets () handling
// - Add mod/pow handling
// - Add scientific notation handling

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

    list<std::string> Translate(const list<std::string>& tokens) const;

    list<std::string> Tokenize(const std::string_view& expression);
  private:
    typedef std::string_view::const_iterator position;

    static constexpr std::string_view kOperators = "+-/*^%~#";
    static constexpr array<std::string_view, 14> kFunctions = {
      "tg",
      "sin",
      "cos",
      "tan",
      "ctg",
      "cot",
      "exp",
      "atg",
      "asin",
      "acos",
      "atan",
      "acot",
      "actg",
      "sqrt"
    };


    enum class TokenType {
      DIGIT,
      ARG,
      OPERATOR,
      FUNCTION,
      OPEN_BRACKET,
      CLOSE_BRACKET,
      UNKNOWN
    };
    
    void Fix(list<std::string>& dest) noexcept;

    void TryOpenBracket(list<std::string>& dest)  noexcept;
    void ManageBrackets() noexcept;

    void PushBrackets(int count, list<std::string>& dest) noexcept;
    bool PushToken(list<std::string>& dest) noexcept;

    void AdvancePosition() noexcept;
    constexpr bool BracketDiscardCond() const noexcept;
    constexpr bool TokenStopCond() const noexcept;

    TokenType GetTokenType(const position& pos) const noexcept;

    constexpr bool MultiplySkipped() const noexcept;
    constexpr bool BracketSkipped() const noexcept;
    constexpr bool BracketFinished() const noexcept;
    constexpr bool BracketNotOpened() const noexcept;
    constexpr bool BracketsBroken() const noexcept;    
    constexpr bool OneSymboled() const noexcept;
    constexpr bool IsNumeric(TokenType token) const noexcept;
    bool ExprFinished(const std::string& last_token) const noexcept;

    TokenType prev_token_;
    TokenType current_token_;
    int unclosed_;
    int bracket_;
    position pos_;
    position end_;
};
}

#endif  // SRC_MODEL_TRANSLATOR_H_
