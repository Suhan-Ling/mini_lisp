#ifndef BUILTINS_H
#define BUILTINS_H

#include "./value.h"

#include <unordered_map>
#include <string>

extern const std::unordered_map<std::string, BuiltinFuncType*> BUILTIN_PROCS;

using binaryOperatorFuncType = double(double, double);
using compareFuncType = bool(double, double);

BuiltinFuncType __apply;
BuiltinFuncType __display;
BuiltinFuncType __displayln;
BuiltinFuncType __error;
BuiltinFuncType __eval;
BuiltinFuncType __exit;
BuiltinFuncType __newline;
BuiltinFuncType __print;

BuiltinFuncType __atom_;
BuiltinFuncType __boolean_;
BuiltinFuncType __integer_;
BuiltinFuncType __list_;
BuiltinFuncType __number_;
BuiltinFuncType __null_;
BuiltinFuncType __pair_;
BuiltinFuncType __procedure_;
BuiltinFuncType __string_;
BuiltinFuncType __symbol_;

BuiltinFuncType __append;
BuiltinFuncType __car;
BuiltinFuncType __cdr;
BuiltinFuncType __cons;
BuiltinFuncType __length;
BuiltinFuncType __list;
BuiltinFuncType __map;
BuiltinFuncType __filter;
BuiltinFuncType __reduce;

BuiltinFuncType __add;
BuiltinFuncType __minus;
BuiltinFuncType __multiply;
BuiltinFuncType __divide;
BuiltinFuncType __abs;
BuiltinFuncType __expt;
BuiltinFuncType __quotient;
BuiltinFuncType __modulo;
BuiltinFuncType __remainder;

BuiltinFuncType __eq_;
BuiltinFuncType __equal_;
BuiltinFuncType __not;
BuiltinFuncType __equal;
BuiltinFuncType __more;
BuiltinFuncType __less;
BuiltinFuncType __moreOrEqual;
BuiltinFuncType __lessOrEqual;
BuiltinFuncType __even_;
BuiltinFuncType __odd_;
BuiltinFuncType __zero_;

// string
BuiltinFuncType __string;
BuiltinFuncType __atoi;
BuiltinFuncType __strlen;

// list
BuiltinFuncType __index;
BuiltinFuncType __insert;
BuiltinFuncType __erase;

// math
BuiltinFuncType __pow;
BuiltinFuncType __log;
BuiltinFuncType __ln;
BuiltinFuncType __log2;

// matrix
BuiltinFuncType __matrix;
BuiltinFuncType __matrix_;
BuiltinFuncType __asmat;
BuiltinFuncType __sub;
BuiltinFuncType __shape;
BuiltinFuncType __minor;
BuiltinFuncType __det;

#endif
