#ifndef SRC_MODEL_I_TRANSLATION_MODEL_H_
#define SRC_MODEL_I_TRANSLATION_MODEL_H_
#include <string>
#include <string_view>

#include "../containers/s21_list.h"

namespace s21 {
class ITranslationModel {
  public:
    virtual list<std::string> Translate(const list<std::string>& tokens) const = 0;
    virtual list<std::string> Tokenize(const std::string_view& expression) = 0;
};
}  // namespace s21

#endif  // SRC_MODEL_I_TRANSLATION_MODEL_H_