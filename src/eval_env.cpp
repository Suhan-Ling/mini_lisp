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
    parent = nullptr;
    for (auto i: BUILTIN_PROCS) {
        symbolTable[i.first] = std::make_shared<BuiltinProcValue>(i.second);
    }
}

EvalEnv::EvalEnv(EnvPtr p) {
    parent = std::move(p);
}

EnvPtr EvalEnv::createGlobal() {
    return std::make_shared<EvalEnv>(EvalEnv());
}

EnvPtr EvalEnv::createChild(EnvPtr p) {
    return std::make_shared<EvalEnv>(EvalEnv(p));
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSelfEvaluating()) {
        return expr;
    } else if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    } else if (auto name = expr->asSymbol()) {
        if (auto value = lookupBinding(*name)) {
            return value;
        } else {
            throw LispError("Variable " + *name + " not defined.");
        }
    } else if (expr->isPair()) {
        auto car = expr->getCar();
        auto cdr = expr->getCdr();
        auto name = car->asSymbol();
        auto form = SPECIAL_FORMS.find(*name);
        if ((name) and (form != SPECIAL_FORMS.end())){
            return (form->second)(cdr->toVector(), *this);
        } else {
            ValuePtr proc = this->eval(car);
            std::vector<ValuePtr> args = evalList(cdr);
            return this->apply(proc, args);
        }
    } else {
        throw LispError("Unimplemented");
    }
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr>& args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        return proc->apply(args, *this);
    } else if (typeid(*proc) == typeid(LambdaValue)) {
        return proc->apply(args, *this);
    } else {
        throw LispError("Not a procedure " + proc->toString() + ".");
    }
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(),
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}

void EvalEnv::defineBinding(std::string name, ValuePtr v) {
    symbolTable[name] = eval(v);
}

ValuePtr EvalEnv::lookupBinding(std::string name) {
    if (auto value = this->symbolTable[name]) {
        return value;
    }
    if (this->parent) {
        return this->parent->lookupBinding(name);
    } else {
        return nullptr;
    }
}