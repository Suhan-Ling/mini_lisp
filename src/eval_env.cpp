#include "./error.h"
#include "./value.h"
#include "./eval_env.h"
#include "./builtins.h"
#include "./forms.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <unordered_map>

using namespace std::literals;

EvalEnv::EvalEnv() {
    for (auto i: BUILTIN_PROC) {
        symbolTable[i.first] = std::make_shared<BuiltinProcValue>(i.second);
    }
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSelfEvaluating()) {
        return expr;
    } else if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    } else if (auto name = expr->asSymbol()) {
        if (auto value = symbolTable[*name]) {
            return value;
        } else {
            throw LispError("Variable " + *name + " not defined.");
        }
    } else if (expr->isPair()) {
        if (auto name = expr->getCar()->asSymbol()) {
            if (SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {
                return SPECIAL_FORMS[*name](expr->getCdr()->toVector(), *this);
            }
        } else {
            ValuePtr proc = this->eval(expr->getCar());
            std::vector<ValuePtr> args = evalList(expr->getCdr());
            return this->apply(proc, args);
        }
    } else {
        throw LispError("Unimplemented");
    }
    return std::make_shared<NilValue>();    // make gcc happy
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        return proc->apply(args);
    } else {
        throw LispError("Not a procedure " + proc->toString());
    }
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(),
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}

void EvalEnv::addSymbol(std::string name, ValuePtr v) {
    symbolTable[name] = eval(v);
}