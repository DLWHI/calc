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

    list<std::string> Translate(const list<std::string>& tokens) const;

    list<std::string> Tokenize(const std::string_view& expression);
  private:
    typedef std::string_view::const_iterator position;

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
    
    void TryOpenBracket(list<std::string>& dest)  noexcept;
    void TryPlaceMultiply(list<std::string>& dest)  noexcept;
    void ManageBrackets() noexcept;

    void PushBrackets(int count, list<std::string>& dest) noexcept;
    bool PushToken(list<std::string>& dest) noexcept;

    template <typename Predicate>
    void MovePos(Predicate condition) noexcept;
    void MoveFunction() noexcept;

    TokenType GetTokenType(const position& pos) const noexcept;

    constexpr bool skipsMultiplyRhs(TokenType token) const noexcept;
    constexpr bool skipsMultiplyLhs(TokenType token) const noexcept;
    constexpr bool isBracketFinisher(TokenType token) const noexcept;
    constexpr bool isNumeric(TokenType token) const noexcept;
    constexpr bool isOneSymboled(TokenType token) const noexcept;
    constexpr bool finishesExpr(TokenType token) const noexcept;
    constexpr bool isBracketsBroken() const noexcept;    

    TokenType prev_token_;
    TokenType current_token_;
    int unclosed_;
    int bracket_;
    position pos_;
    position end_;

};
}

#endif  // SRC_MODEL_TRANSLATOR_H_
