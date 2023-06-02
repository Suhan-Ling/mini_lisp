#ifndef BUILTINS_H
#define BUILTINS_H

#include "./value.h"

#include <unordered_map>
#include <string>

extern const std::unordered_map<std::string, BuiltinFuncType*> BUILTIN_PROCS;

using binaryOperatorFuncType = double(double, double);
using compareFuncType = bool(double, double);

BuiltinFuncType print;
BuiltinFuncType atom_;
BuiltinFuncType boolean_;
BuiltinFuncType integer_;
BuiltinFuncType number_;
BuiltinFuncType null_;
BuiltinFuncType pair_;
BuiltinFuncType procedure_;
BuiltinFuncType string_;
BuiltinFuncType symbol_;
BuiltinFuncType add;
BuiltinFuncType substract;
BuiltinFuncType multiply;
BuiltinFuncType divide;
BuiltinFuncType _equal;
BuiltinFuncType _more;
BuiltinFuncType _less;
BuiltinFuncType _moreOrEqual;
BuiltinFuncType _lessOrEqual;

#endif