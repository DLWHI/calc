#ifndef SRC_MODEL_BAD_EXPRESSION_H_
#define SRC_MODEL_BAD_EXPRESSION_H_

#include <stdexcept>
#include <string>

namespace s21 {
class BadExpression final : public std::runtime_error {
 public:
  BadExpression(const std::string& what) : std::runtime_error(what){};
};
}  // namespace s21

#endif  // SRC_MODEL_BAD_EXPRESSION_H_
