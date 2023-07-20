#ifndef SRC_MODEL_TRANSLATOR_H_
#define SRC_MODEL_TRANSLATOR_H_

#include "ITranslationModel.h"
#include "../containers/array.h"
#include "../containers/s21_stack.h"

namespace s21 {
class Translator : public ITranslationModel  {
  public:
    static constexpr size_t kMaxExprLength = 255;

    list<std::string> Translate(const list<std::string>& tokens) const;

    list<std::string> Tokenize(const std::string_view& expression);
  private:
    typedef std::string_view::const_iterator position;

    static constexpr std::string_view kOperators = "+-/*^%#~";
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
      kDigit,
      kArg,
      kOperator,
      kFunction,
      kOpenBracket,
      kCloseBracket,
      kUnknown
    };
    
    enum class State {
      kPush,
      kDiscard,
      kFunctionErr,
      kLonelyOperator,
      kBrokenBrackets
    };

    void Fix(list<std::string>& dest) noexcept;

    void CloseBracket() noexcept;
    void CollapseOperator(list<std::string>& dest) noexcept;
    bool PushToken(list<std::string>& dest) noexcept;

    void AdvancePosition() noexcept;
    bool IsDigit() const noexcept;
    void ThrowErrors(const std::string_view& last_token) const;

    TokenType GetTokenType(const position& pos) const noexcept;
    char OpUnary(const position& op) const noexcept;
    char OpBinary(const position& op) const noexcept;

    constexpr bool ValidState() const noexcept;
    constexpr bool MultiplySkipped() const noexcept;
    constexpr bool FunctionEmpty() const noexcept;
    constexpr bool BracketSkipped() const noexcept;
    constexpr bool BracketFinished() const noexcept;
    constexpr bool BracketsBroken() const noexcept;    
    constexpr bool OneSymboled() const noexcept;
    constexpr bool IsNumeric(TokenType token) const noexcept;
    constexpr bool ExprFinished(TokenType last_token) const noexcept;

    TokenType prev_token_;
    TokenType current_token_;
    State push_;
    stack<char> brackets_;
    position pos_;
    position end_;
};
}

#endif  // SRC_MODEL_TRANSLATOR_H_
