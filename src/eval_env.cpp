#include "./error.h"
#include "./value.h"
#include "./eval_env.h"
#include "./builtins.h"

#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std::literals;

EvalEnv::EvalEnv() {
    symbolTable["print"]        = std::make_shared<BuiltinProcValue>(&print);
    symbolTable["atom?"]        = std::make_shared<BuiltinProcValue>(&atom_);
    symbolTable["boolean?"]     = std::make_shared<BuiltinProcValue>(&boolean_);
    symbolTable["integer?"]     = std::make_shared<BuiltinProcValue>(&integer_);
    symbolTable["number?"]      = std::make_shared<BuiltinProcValue>(&number_);
    symbolTable["null?"]        = std::make_shared<BuiltinProcValue>(&null_);
    symbolTable["pair?"]        = std::make_shared<BuiltinProcValue>(&pair_);
    symbolTable["procedure?"]   = std::make_shared<BuiltinProcValue>(&procedure_);
    symbolTable["string?"]      = std::make_shared<BuiltinProcValue>(&string_);
    symbolTable["symbol?"]      = std::make_shared<BuiltinProcValue>(&symbol_);
    symbolTable["+"]            = std::make_shared<BuiltinProcValue>(&add);
    symbolTable["-"]            = std::make_shared<BuiltinProcValue>(&substract);
    symbolTable["*"]            = std::make_shared<BuiltinProcValue>(&multiply);
    symbolTable["/"]            = std::make_shared<BuiltinProcValue>(&divide);
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
        std::vector<ValuePtr> v = expr->toVector();
        if (v[0]->asSymbol() == "define"s) {
            if (auto name = v[1]->asSymbol()) {
                symbolTable[*name] = eval(v[2]);
                return std::make_shared<NilValue>();
            } else {
                throw LispError("Malformed define.");
            }
        } else {
            ValuePtr proc = this->eval(v[0]);
            // std::cout << "eval:" << std::endl;
            // std::cout << v.size() << std::endl;
            // for (auto i: v) {
            //     std::cout << i->toString() << std::endl;
            // }
            // std::cout << std::endl;
            // std::cout << expr->getRight()->toString() << std::endl << std::endl;
            std::vector<ValuePtr> args = evalList(expr->getRight());
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
        throw LispError("Unimplemented");
    }
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(),
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}