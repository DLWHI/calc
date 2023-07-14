#ifndef SRC_MODEL_I_TRANSLATION_MODEL_H_
#define SRC_MODEL_I_TRANSLATION_MODEL_H_
#include "../containers/vector.h"
#include <string>

namespace s21 {
class ITranslationModel {
  public:
    virtual vector<std::string> Translate(const std::string& expression) const = 0;
    virtual void Tokenize(const std::string& expression) const = 0;
};
}

#endif  // SRC_MODEL_I_TRANSLATION_MODEL_H_