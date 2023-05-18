#include "./value.h"

#include <iomanip>
#include <cmath>
#include <typeinfo>
#include <iostream>

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
        return "(" + result + ")";
    }
}