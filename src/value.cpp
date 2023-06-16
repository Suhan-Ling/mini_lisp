#include "./value.h"
#include "./error.h"
#include "./eval_env.h"
#include "./builtins.h"

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

bool Value::isList() const {
    return (typeid(*this) == typeid(NilValue));
}

bool Value::isMatrix() const {
    return (typeid(*this) == typeid(NilValue));
}

bool Value::isSelfEvaluating() const {
    return (typeid(*this) == typeid(BooleanValue)) or
           (typeid(*this) == typeid(NumericValue)) or
           (typeid(*this) == typeid(StringValue)) or
           (typeid(*this) == typeid(BuiltinProcValue)) or
           (typeid(*this) == typeid(LambdaValue));
}

int Value::length() const {
    if (!this->isNil()) {
        throw LispError("Malformed list: expected pair or nil, got " +
                        this->toString());
    }
    return 0;
}

ValuePtr Value::shape() const {
    if (!this->isNil()) {
        throw LispError("Malformed matrix: expected list, got " +
                        this->toString());
    }
    return std::make_shared<PairValue>(std::make_shared<NumericValue>(0), 
                                std::make_shared<NumericValue>(0));
}

void Value::checkRange(int x, int y) const {
    throw LispError("Subsript out of range.");
}

int Value::rowNum() const {
    if (!this->isNil()) {
        throw LispError("Malformed matrix: expected list, got " +
                        this->toString());
    }
    return 0;
}

int Value::colNum() const {
    if (!this->isNil()) {
        throw LispError("Malformed matrix: expected list, got " +
                        this->toString());
    }
    return 0;
}

int Value::sub(int x, int y) const {
    if (!this->isNil()) {
        throw LispError("Malformed matrix: expected list, got " +
                        this->toString());
    }
    throw LispError("Subscript out of range.");
}

ValuePtr Value::minor(int x, int y) const {
    if (!this->isNil()) {
        throw LispError("Malformed matrix: expected list, got " +
                        this->toString());
    }
    throw LispError("Empty matrix don't have minor.");
}

int Value::det() const {
    if (!this->isNil()) {
        throw LispError("Malformed matrix: expected list, got " +
                        this->toString());
    }
    throw LispError("Empty matrix don't have determine.");
}

std::vector<ValuePtr> Value::toVector() const {
    throw LispError("Cannot convert to vector.");
}

std::vector<ValuePtr> Value::asList() const {
    if (!this->isNil()) {
        throw LispError("Malformed list: expected pair or nil, got " +
                        this->toString());
    }
    return std::vector<ValuePtr>();
}

std::vector<ValuePtr> Value::asRow() const {
    if (!this->isNil()) {
        throw LispError("Malformed matrix: expected list, got " +
                        this->toString());
    }
    return std::vector<ValuePtr>();
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

ValuePtr Value::apply(const std::vector<ValuePtr>& args, EvalEnv& env) const {
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

bool PairValue::isList() const {
    ValuePtr cdr = this->cdr;
    while (1) {
        if (cdr->isNil()) {
            return true;
        }
        if (!cdr->isPair()) {
            return false;
        }
        cdr = cdr->getCdr();
    }
}

bool PairValue::isMatrix() const {
    if (!this->isList()) {
        return false;
    }
    std::vector<ValuePtr> row = this->asList();
    std::vector<ValuePtr> list;
    int len = 0;
    for (auto i: row) {
        if (!i->isList()) {
            return false;
        }
        list = i->asList();
        if ((i != *row.begin()) and (len != list.size())) {
            return false;
        }
        len = list.size();
        for (auto j: list) {
            if (!j->isNumber()) {
                return false;
            }
        }
    }
    return true;
}

std::string PairValue::getType() const {
    return "PairValue";
}

int PairValue::length() const {
    int cnt = 1;
    ValuePtr cdr = this->cdr;
    while (1) {
        if (cdr->isNil()) {
            break;
        }
        if (!cdr->isPair()) {
            throw LispError("Malformed list: expected pair or nil, got " +
                            cdr->toString());
        }
        cnt++;
        cdr = cdr->getCdr();
    }
    return cnt;
}

ValuePtr PairValue::shape() const {
    if (!this->isList()) {
        throw LispError("Malformed matrix: expected list, got " +
                        this->toString());
    }
    std::vector<ValuePtr> row = this->asList();
    std::vector<ValuePtr> list;
    int len = 0;
    for (auto i: row) {
        if (!i->isList()) {
            throw LispError("Malformed matrix: expected list, got " +
                            i->toString());
        }
        list = i->asList();
        if ((i != *row.begin()) and (len != list.size())) {
            throw LispError("Matrix shape mismatch: expected " + std::to_string(len)
                            + ", got " + std::to_string(list.size()) + ".");
        }
        len = list.size();
        for (auto j: list) {
            if (!j->isNumber()) {
                throw LispError("Malformed matrix: expected number, got " +
                                j->toString());
            }
        }
    }
    ValuePtr r, c;
    r = std::make_shared<NumericValue>(row.size());
    c = std::make_shared<NumericValue>(len);
    return std::make_shared<PairValue>(r, c);
}

void PairValue::checkRange(int x, int y) const {
    int r = this->rowNum();
    int c = this->colNum();
    if ((x < 0) or (x >= r)) {
        throw LispError("Subscript out of range.");
    }
    if ((y < 0) or (y >= c)) {
        throw LispError("Subscript out of range.");
    }
}

int PairValue::rowNum() const {
    return this->shape()->getCar()->asNumber();
}

int PairValue::colNum() const {
    return this->shape()->getCdr()->asNumber();
}

int PairValue::sub(int x, int y) const {
    this->checkRange(x, y);
    std::vector<ValuePtr> row = this->asRow();
    std::vector<ValuePtr> list = row[x]->asList();
    return list[y]->asNumber();
}

ValuePtr PairValue::minor(int x, int y) const {
    this->checkRange(x, y);
    std::vector<ValuePtr> row = this->asRow();
    row.erase(row.begin() + x);
    for (auto& i: row) {
        std::vector<ValuePtr> list = i->asList();
        list.erase(list.begin() + y);
        ValuePtr cdr = std::make_shared<NilValue>();
        for (int j = list.size() - 1; j >= 0; j--) {
            cdr = std::make_shared<PairValue>(list[j], cdr);
        }
        i = cdr;
    }
    ValuePtr cdr = std::make_shared<NilValue>();
    for (int j = row.size() - 1; j >= 0; j--) {
        cdr = std::make_shared<PairValue>(row[j], cdr);
    }
    return cdr;
}

int PairValue::det() const {
    int len = this->rowNum();
    if (len != this->colNum()) {
        throw LispError("Not a square matrix.");
    }
    if (len == 1) {
        return this->getCar()->getCar()->asNumber();
    }
    int result = 0;
    for (int i = 0; i < len; i++) {
        if (i % 2 == 0) {
            result += this->sub(0, i) * this->minor(0, i)->det();
        } else {
            result -= this->sub(0, i) * this->minor(0, i)->det();
        }
    }
    return result;
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

std::vector<ValuePtr> PairValue::asList() const {
    ValuePtr car;
    ValuePtr cdr = this->cdr;
    std::vector<ValuePtr> result;
    result.push_back(this->car);
    while (1) {
        if (cdr->isNil()) {
            break;
        }
        if (!cdr->isPair()) {
            throw LispError("Malformed list: expected pair or nil, got " +
                            cdr->toString());
        }
        car = cdr->getCar();
        cdr = cdr->getCdr();
        result.push_back(car);
    }
    return result;
}

std::vector<ValuePtr> PairValue::asRow() const {
    ValuePtr car = this->car;
    ValuePtr cdr = this->cdr;
    if (!car->isList()) {
        throw LispError("Malformed matrix: expected list, got " +
                        car->toString());
    }
    int len = car->length();
    std::vector<ValuePtr> result;
    result.push_back(car);
    while (1) {
        if (cdr->isNil()) {
            break;
        }
        if (!cdr->isPair()) {
            throw LispError("Malformed matrix: expected list, got " +
                            this->toString());
        }
        car = cdr->getCar();
        if (!car->isList()) {
            throw LispError("Malformed matrix: expected list, got " +
                            car->toString());
        }
        if (len != car->length()) {
            throw LispError("Matrix shape mismatch: expected " + std::to_string(len)
                            + ", got " + std::to_string(car->length()) + ".");
        }
        cdr = cdr->getCdr();
        result.push_back(car);
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

ValuePtr BuiltinProcValue::apply(const std::vector<ValuePtr>& args, EvalEnv& env) const{
    return func(args, env);
}

std::string LambdaValue::toString() const{
    return "#<procedure: lambda>";
}

std::string LambdaValue::getType() const {
    return "LambdaValue";
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args, EvalEnv& env) const {
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
