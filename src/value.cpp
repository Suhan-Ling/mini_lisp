#include "./value.h"
#include "./error.h"
#include "./eval_env.h"

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
    return (typeid(*this) == typeid(BooleanValue)) or
           (typeid(*this) == typeid(NumericValue)) or
           (typeid(*this) == typeid(StringValue)) or
           (typeid(*this) == typeid(BuiltinProcValue)) or
           (typeid(*this) == typeid(LambdaValue));
}

bool Value::isPair() const {
    return (typeid(*this) == typeid(PairValue));
}

bool Value::isNumber() const {
    return (typeid(*this) == typeid(NumericValue));
}

std::vector<ValuePtr> Value::toVector() const {
    throw LispError("Cannot convert to vector.");
}

std::optional<std::string> Value::asSymbol() const {
    return std::nullopt;
}

double Value::asNumber() const {
    throw LispError("Not a number.");
}

ValuePtr Value::getCar() const {
    throw LispError("Not a pair value.");
}

ValuePtr Value::getCdr() const {
    throw LispError("Not a pair value.");
}

ValuePtr Value::apply(const std::vector<ValuePtr>& args, EvalEnv& env) {
    throw LispError("Not a procedure.");
}

std::string BooleanValue::toString() const {
    if (value) {
        return "#t";
    } else {
        return "#f";
    }
}

std::string BooleanValue::getType() const {
    return "BooleanValue";
}

std::string NumericValue::toString() const {
    if (std::floor(value) == value) {
        return std::to_string(static_cast<int>(value));
    } else {
        return std::to_string(value);
    }
}

std::string NumericValue::getType() const {
    return "NumericValue";
}

double NumericValue::asNumber() const {
    return value;
}

std::string StringValue::toString() const {
    std::stringstream ss;
    ss << std::quoted(value);
    return ss.str();
}

std::string StringValue::getType() const {
    return "StringValue";
}

std::string NilValue::toString() const {
    return "()";
}

std::string NilValue::getType() const {
    return "NilValue";
}

std::vector<ValuePtr> NilValue::toVector() const {
    std::vector<ValuePtr> v;
    return v;
}

std::string SymbolValue::toString() const {
    return name;    
}

std::optional<std::string> SymbolValue::asSymbol() const {
    return name;
}

std::string SymbolValue::getType() const {
    return "SymbolValue";
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
        // printf("111\n");
        return "(" + result + ")";
    }
}

std::string PairValue::getType() const {
    return "PairValue";
}

std::vector<ValuePtr> PairValue::toVector() const {
    std::vector<ValuePtr> result, r;
    result.push_back(left);
    if (right->isPair()) {
        r = right->toVector();
        for (auto i: r) {
            result.push_back(i);
        }
    } else if (right->getType() != "NilValue") {
        result.push_back(right);
    }
    return result;
}

ValuePtr PairValue::getCar() const {
    return left;
}

ValuePtr PairValue::getCdr() const {
    return right;
}


std::string BuiltinProcValue::toString() const {
    return "#<procedure>"; 
}

std::string BuiltinProcValue::getType() const {
    return "BuiltinProcValue";
}

ValuePtr BuiltinProcValue::apply(const std::vector<ValuePtr>& args, EvalEnv& env) {
    return func(args, env);
}

std::string LambdaValue::toString() const{
    return "#<procedure>";
}

std::string LambdaValue::getType() const {
    return "LambdaValue";
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args, EvalEnv& env) {
    EnvPtr envChild = EvalEnv::createChild(this->env);
    int paramsLen = params.size();
    int argsLen = args.size();
    if (paramsLen != argsLen) {
        throw LispError("Procedure expected " + std::to_string(paramsLen) +
                        " parameters, got " + std::to_string(argsLen) + ".");
    }
    for (int i = 0; i < argsLen; i++) {
        envChild->defineBinding(params[i], args[i]);
    }
    ValuePtr value;
    for (auto i: body) {
        value = envChild->eval(i);
    }
    return value;
}