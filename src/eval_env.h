#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <unordered_map>
#include <vector>

#include "./value.h"
#include "./builtins.h"

class EvalEnv: public std::enable_shared_from_this<EvalEnv> {
public:
    std::unordered_map<std::string, ValuePtr> symbolTable;
    EnvPtr parent;
    EvalEnv();
    EvalEnv(EnvPtr p);
public:
    static EnvPtr createGlobal();
    static EnvPtr createChild(EnvPtr p);
    ValuePtr eval(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    std::vector<ValuePtr> evalList(ValuePtr expr);
    void defineBinding(std::string name, ValuePtr v);
    ValuePtr lookupBinding(std::string name);
};

#endif