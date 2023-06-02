#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <unordered_map>
#include <vector>

#include "./value.h"
#include "./builtins.h"

class EvalEnv: public std::enable_shared_from_this<EvalEnv> {
public:
    std::unordered_map<std::string, ValuePtr> symbolTable;
    std::shared_ptr<EvalEnv> parent;
public:
    EvalEnv();
    EvalEnv(EvalEnv& p);
    // static std::shared_ptr<EvalEnv> createGlobal();
    // static std::shared_ptr<EvalEnv> createGlobal(EvalEnv& p);
    ValuePtr eval(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    std::vector<ValuePtr> evalList(ValuePtr expr);
    void defineBinding(std::string name, ValuePtr v);
    ValuePtr lookupBinding(std::string name);
};

#endif