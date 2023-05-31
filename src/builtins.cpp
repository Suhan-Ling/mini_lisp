#include "./value.h"
#include "./error.h"
#include "./builtins.h"

#include <iostream>
#include <cmath>
#include <string>

ValuePtr print(const std::vector<ValuePtr>& params) {
    for (const auto& i: params) {
        std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr atom_(const std::vector<ValuePtr>& params) {
    bool b;
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
    bool b;
    b = (params[0]->getType() == "NumericValue") and 
        (params[0]->asNumber() == std::floor(params[0]->asNumber()));
    return std::make_shared<BooleanValue>(b);
}

ValuePtr number_(const std::vector<ValuePtr>& params) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "NumericValue");
}

ValuePtr null_(const std::vector<ValuePtr>& params) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "NilValue");
}

ValuePtr pair_(const std::vector<ValuePtr>& params) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "PairValue");
}

ValuePtr procedure_(const std::vector<ValuePtr>& params) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "BuiltinProcValue");
}

ValuePtr string_(const std::vector<ValuePtr>& params) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "StringValue");
}

ValuePtr symbol_(const std::vector<ValuePtr>& params) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "SymbolValue");
}

ValuePtr add(const std::vector<ValuePtr>& params) {
    int result = 0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError(i->toString() + " is nut number.");
        }
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr substract(const std::vector<ValuePtr>& params) {
    int len = params.size();
    if (len == 2) {
        if (!params[0]->isNumber()) {
            throw LispError(params[0]->toString() + " is nut number.");
        } else if (!params[1]->isNumber()) {
            throw LispError(params[1]->toString() + " is nut number.");
        }
        return std::make_shared<NumericValue>(params[0]->asNumber() - params[1]->asNumber());
    } else if (len == 1) {
        if (!params[0]->isNumber()) {
            throw LispError(params[0]->toString() + " is nut number.");
        }
        return std::make_shared<NumericValue>(0 - params[0]->asNumber());
    } else if (len > 2) {
        throw LispError("Too many arguments: " + std::to_string(len) + " > 2.");
    } else {    // len == 0
        throw LispError("Too few arguments: 0 < 1.");
    }
}

ValuePtr multiply(const std::vector<ValuePtr>& params) {
    int result = 1;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError(i->toString() + " is nut number.");
        }
        result *= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divide(const std::vector<ValuePtr>& params) {
    int len = params.size();
    if (len == 2) {
        if (!params[0]->isNumber()) {
            throw LispError(params[0]->toString() + " is nut number.");
        } else if (!params[1]->isNumber()) {
            throw LispError(params[1]->toString() + " is nut number.");
        } else if (params[1]->asNumber() == 0) {
            throw LispError("Cannot divide 0.");
        }
        return std::make_shared<NumericValue>(params[0]->asNumber() / params[1]->asNumber());
    } else if (len == 1) {
        if (!params[0]->isNumber()) {
            throw LispError(params[0]->toString() + " is nut number.");
        } else if (params[0]->asNumber() == 0) {
            throw LispError("Cannot divide 0.");
        }
        return std::make_shared<NumericValue>(1 / params[0]->asNumber());
    } else if (len > 2) {
        throw LispError("Too many arguments: " + std::to_string(len) + " > 2.");
    } else {    // len == 0
        throw LispError("Too few arguments: 0 < 1.");
    }
}