#include "./value.h"
#include "./error.h"
#include "./eval_env.h"

#include <iomanip>
#include <cmath>
#include <typeinfo>
#include <iostream>
#include <typeinfo>
#include <functional>

bool isInt(double value) {
    return std::floor(value) == value;
}

bool Value::isBool() const {
    return (typeid(*this) == typeid(BooleanValue));
}

bool Value::isNumber() const {
    return (typeid(*this) == typeid(NumericValue));
}

bool Value::isString() const {
    return (typeid(*this) == typeid(StringValue));
}

bool Value::isNil() const {
    return (typeid(*this) == typeid(NilValue));
}

bool Value::isSymbol() const {
    return (typeid(*this) == typeid(SymbolValue));
}

bool Value::isPair() const {
    return (typeid(*this) == typeid(PairValue));
}

bool Value::isProc() const {
    return (typeid(*this) == typeid(BuiltinProcValue));
}

bool Value::isLambda() const {
    return (typeid(*this) == typeid(LambdaValue));
}

bool Value::isSelfEvaluating() const {
    return (typeid(*this) == typeid(BooleanValue)) or
           (typeid(*this) == typeid(NumericValue)) or
           (typeid(*this) == typeid(StringValue)) or
           (typeid(*this) == typeid(BuiltinProcValue)) or
           (typeid(*this) == typeid(LambdaValue));
}

std::vector<ValuePtr> Value::toVector() const {
    throw LispError("Cannot convert to vector.");
}

std::optional<std::string> Value::asSymbol() const {
    return std::nullopt;
}

double Value::asNumber() const {
    throw LispError(this->toString() + " is not number.");
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
    if (isInt(value)) {
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

std::string PairValue::toString() const {
    std::string left = car->toString();
    std::string right;
    if (cdr->isNil()) {
        right = "";
    } else if (cdr->isPair()) {
        right = cdr->toString();
        right.replace(0, 1, " ");
        right.pop_back();
    } else {
        right = " . " + cdr->toString();
    }
    return "(" + left + right + ")";
}

std::string PairValue::getType() const {
    return "PairValue";
}

std::vector<ValuePtr> PairValue::toVector() const {
    std::vector<ValuePtr> result, r;
    result.push_back(car);
    if (cdr->isPair()) {
        r = cdr->toVector();
        for (auto i: r) {
            result.push_back(i);
        }
    } else if (!cdr->isNil()) {
        result.push_back(cdr);
    }
    return result;
}

ValuePtr PairValue::getCar() const {
    return car;
}

ValuePtr PairValue::getCdr() const {
    return cdr;
}


std::string BuiltinProcValue::toString() const {
    return "#<procedure: builtin>"; 
}

std::string BuiltinProcValue::getType() const {
    return "BuiltinProcValue";
}

ValuePtr BuiltinProcValue::apply(const std::vector<ValuePtr>& args, EvalEnv& env) {
    return func(args, env);
}

std::string LambdaValue::toString() const{
    return "#<procedure: lambda>";
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