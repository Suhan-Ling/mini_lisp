#include "./value.h"
#include "./error.h"

#include <iomanip>
#include <cmath>
#include <typeinfo>
#include <iostream>
#include <typeinfo>
#include <functional>

bool Value::isNil() const {
    return (typeid(*this) == typeid(NilValue));
}

bool Value::isSelfEvaluating() const {
    return  (typeid(*this) == typeid(BooleanValue)) or (typeid(*this) == typeid(NumericValue)) or (typeid(*this) == typeid(StringValue));
} 

bool Value::isList() const {
    return (typeid(*this) == typeid(PairValue));
}

std::vector<ValuePtr> Value::toVector() const {
    throw LispError("Value is not a list.");
}

std::optional<std::string> Value::asSymbol() const {
    return std::nullopt;
}

std::string BooleanValue::toString() const {
    if (value) {
        return "#t";
    } else {
        return "#f";
    }
}

std::string NumericValue::toString() const {
    if (std::floor(value) == value) {
        return std::to_string(static_cast<int>(value));
    } else {
        return std::to_string(value);
    }
}

std::string StringValue::toString() const {
    std::stringstream ss;
    ss << std::quoted(value);
    return ss.str();
}

std::string NilValue::toString() const {
    return "()";
}

std::string SymbolValue::toString() const {
    return name;    
}

std::optional<std::string> SymbolValue::asSymbol() const {
    return name;
}

static int PairValue_toString_layer = 0;

std::string PairValue::toString() const {
    PairValue_toString_layer++;

    std::string result = "";
    if (right->toString() == "()") {
        result += left->toString();
    } else if (auto rightPair = dynamic_cast<PairValue*>(right.get())) {
        result += left->toString() + " " + rightPair->toString();
    } else {
        result += left->toString() + " . " + right->toString();
    }
    
    PairValue_toString_layer--;
    if (PairValue_toString_layer) {
        return result;
    } else {
        printf("111\n");
        return "(" + result + ")";
    }
}

std::vector<ValuePtr> PairValue::toVector() const {
    std::vector<ValuePtr> result, r;
    result.push_back(left);
    if (right->isList()) {
        r = right->toVector();
        for (auto i: r) {
            result.push_back(i);
        }
    } else {
        result.push_back(right);
    }
    return result;
}