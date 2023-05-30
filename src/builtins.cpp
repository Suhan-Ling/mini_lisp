#include "./value.h"
#include "./error.h"
#include "./builtins.h"

void addTable(std::unordered_map<std::string, ValuePtr> t, std::string s, BuiltinFuncType* f) {
    t[s] = std::make_shared<BuiltinProcValue>(f);
}

ValuePtr add(const std::vector<ValuePtr>& params) {
    int result = 0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}