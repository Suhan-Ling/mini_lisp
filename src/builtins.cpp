#include "./value.h"
#include "./error.h"
#include "./builtins.h"

#include <iostream>

ValuePtr print(const std::vector<ValuePtr>& params) {
    for (const auto& i: params) {
        std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr atom_(const std::vector<ValuePtr>& params) {
    boolean b;
    b = (params[0]->getType() == "BooleanValue") or
        (params[0]->getType() == "NumericValue") or
        (params[0]->getType() == "StringValue") or
        (params[0]->getType() == "SymbolValue") or
        (params[0]->getType() == "NilValue");
    return std::make_shared<BooleanValue>(b);
}

ValuePtr boolean_(const std::vector<ValuePtr>& params) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "BooleanValue");
}

ValuePtr integer_(const std::vector<ValuePtr>& params) {
    
}

ValuePtr list_(const std::vector<ValuePtr>& params);

ValuePtr number_(const std::vector<ValuePtr>& params);

ValuePtr null_(const std::vector<ValuePtr>& params);

ValuePtr pair_(const std::vector<ValuePtr>& params);

ValuePtr procedure_(const std::vector<ValuePtr>& params);

ValuePtr string_(const std::vector<ValuePtr>& params);

ValuePtr symbol_(const std::vector<ValuePtr>& params);

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

ValuePtr substract(const std::vector<ValuePtr>& params) {
    int result = 0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot substract a non-numeric value.");
        }
        result -= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}