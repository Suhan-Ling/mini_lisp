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
    for (auto i: BUILTIN_PROCS) {
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
        auto car = expr->getCar();
        auto cdr = expr->getCdr();
        auto name = car->asSymbol();
        if ((name) and (SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end())){
            return SPECIAL_FORMS[*name](cdr->toVector(), *this);
        } else {
            ValuePtr proc = this->eval(car);
            std::vector<ValuePtr> args = evalList(cdr);
            return this->apply(proc, args);
        }
    } else {
        throw LispError("Unimplemented");
    }
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