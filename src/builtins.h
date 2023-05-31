#ifndef BUILTINS_H
#define BUILTINS_H

#include <unordered_map>

#include "./value.h"

ValuePtr print(const std::vector<ValuePtr>& params);
ValuePtr atom_(const std::vector<ValuePtr>& params);
ValuePtr boolean_(const std::vector<ValuePtr>& params);
ValuePtr integer_(const std::vector<ValuePtr>& params);
ValuePtr list_(const std::vector<ValuePtr>& params);
ValuePtr number_(const std::vector<ValuePtr>& params);
ValuePtr null_(const std::vector<ValuePtr>& params);
ValuePtr pair_(const std::vector<ValuePtr>& params);
ValuePtr procedure_(const std::vector<ValuePtr>& params);
ValuePtr string_(const std::vector<ValuePtr>& params);
ValuePtr symbol_(const std::vector<ValuePtr>& params);
ValuePtr add(const std::vector<ValuePtr>& params);
ValuePtr substract(const std::vector<ValuePtr>& params);

#endif