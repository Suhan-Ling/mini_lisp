#include "./eval_env.h"
#include "./error.h"
#include "./value.h"

using namespace std::literals;

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSelfEvaluating()) {
        return expr;
    } else if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    } else if (auto name = expr->asSymbol()) {
        if (auto value = 符号表.查找(*name)) {
            return value;
        } else {
            throw LispError("Variable " + *name + " not defined.");
        }
    } else if (expr->isList()) {
        std::vector<ValuePtr> v = expr->toVector();
        if (v[0]->asSymbol() == "define"s) {
        if (auto name = v[1]->asSymbol()) {
            将 (*name, v[2]) 添加到符号表中;
            return 空表;
        } else {
            throw LispError("Malformed define.");
        }
    } else {
        throw LispError("Unimplemented");
    }
}