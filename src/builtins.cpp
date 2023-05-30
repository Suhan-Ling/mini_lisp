#include "./builtins.h"
#include "./value.h"
#include "./error.h"

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