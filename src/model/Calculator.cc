#include "Calculator.h"

#include <string_view>
#include <cmath>

#include "../containers/s21_stack.h"

namespace s21 {
  double Calculator::Calculate() {
    stack<double> calc_stack;
    double result = 0;
    for (auto token: expression) {
      if (get_token_type(token.at(0)) == DIGIT ||
            get_token_type(token.at(0)) == ARG)
          s = push_wrapper(s, list_front(postfix), arg_val, error_msg);
        else if (get_token_type((char*)list_front(postfix)) == OPERATOR ||
                get_token_type((char*)list_front(postfix)) == FUNCTION)
          s = operate(list_front(postfix), s, error_msg);
        postfix = postfix->next;
    }
    if (s)
      s = pop_wrapper(s, &result);
    else if (error_msg)
      strcpy(error_msg, "INVALID EXPRESSION");
    stack_destroy(s);
    return result;
  }

  void Calculator::Operate(const std::string& operation, stack<double>& calc_stack) const {
  double l, r;
  l = calc_stack.top();
  calc_stack.pop();
  // make iterate through list of unary functions
  if (!std::string_view("sin").compare(0, 3, operation.c_str(), operation.size()))
    return calc_stack.push(sin(l));
  else if (!strncmp(op, "cos", 3))
    return push_wrapper(pool, "x", cos(l), err_msg);
  else if (!strncmp(op, "tan", 3) || !strncmp(op, "tg", 2))
    return push_wrapper(pool, "x", tan(l), err_msg);
  else if (!strncmp(op, "asin", 4))
    return push_wrapper(pool, "x", asin(l), err_msg);
  else if (!strncmp(op, "acos", 4))
    return push_wrapper(pool, "x", acos(l), err_msg);
  else if (!strncmp(op, "atan", 4) || !strncmp(op, "atg", 3))
    return push_wrapper(pool, "x", atan(l), err_msg);
  else if (!strncmp(op, "sqrt", 4))
    return push_wrapper(pool, "x", sqrt(l), err_msg);
  else if (!strncmp(op, "exp", 3))
    return push_wrapper(pool, "x", exp(l), err_msg);
  else if (!strncmp(op, "ln", 2))
    return push_wrapper(pool, "x", log(l), err_msg);
  else if (!strncmp(op, "log", 3))
    return push_wrapper(pool, "x", log10(l), err_msg);
  else if (*op == '~')
    return push_wrapper(pool, "x", -l, err_msg);
  else if (*op == '#')
    return push_wrapper(pool, "x", l, err_msg);
  else if (!pool)
    return pool;
  pool = pop_wrapper(pool, &r);
  if (*op == '%')
    return push_wrapper(pool, "x", fmod(r, l), err_msg);
  else if (*op == '^')
    return push_wrapper(pool, "x", pow(r, l), err_msg);
  else if (*op == '+')
    return push_wrapper(pool, "x", r + l, err_msg);
  else if (*op == '-')
    return push_wrapper(pool, "x", r - l, err_msg);
  else if (*op == '*')
    return push_wrapper(pool, "x", r * l, err_msg);
  else if (*op == '/')
    return push_wrapper(pool, "x", r / l, err_msg);
  else if (!strncmp(op, "mod", 3))
    return push_wrapper(pool, "x", fmod(r, l), err_msg);
  }

}  // namespace s21
