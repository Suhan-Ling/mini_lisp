#ifndef BUILTINS_H
#define BUILTINS_H

#include <unordered_map>

#include "./value.h"

void addTable(std::unordered_map<std::string, ValuePtr> t, std::string s, BuiltinFuncType* f);

ValuePtr add(const std::vector<ValuePtr>& params);

#endif