#include "./eval_env.h"
#include "./error.h"
#include "./value.h"
#include "./builtins.h"

using namespace std::literals;

EvalEnv::EvalEnv() {
    symbolTable["+"] = std::make_shared<BuiltinProcValue>(&add);
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
    } else if (expr->isList()) {
        std::vector<ValuePtr> v = expr->toVector();
        if (v[0]->asSymbol() == "define"s) {
            if (auto name = v[1]->asSymbol()) {
                symbolTable[*name] = eval(v[2]);
                return std::make_shared<NilValue>();
            } else {
                throw LispError("Malformed define.");
            }
        } else {
            throw LispError("Unimplemented");
        }
    } else {
        throw LispError("Unimplemented");
    }
}