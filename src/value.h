#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <memory>
#include <typeinfo>
#include <vector>
#include <optional>

class Value;
class EvalEnv;
using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);

class Value {
public:
    virtual ~Value() = default;
    virtual std::string toString() const = 0;
    bool isNil() const;
    bool isSelfEvaluating() const;
    bool isPair() const;
    bool isNumber() const;
    virtual std::vector<ValuePtr> toVector() const;
    virtual std::optional<std::string> asSymbol() const;
    virtual double asNumber() const;
    virtual ValuePtr getCar() const;
    virtual ValuePtr getCdr() const;
    virtual ValuePtr apply(std::vector<ValuePtr> args);
    virtual std::string getType() const = 0;
};


class BooleanValue: public Value {
    bool value;
public:
    BooleanValue(bool val): value{val} {}
    std::string toString() const override;
    std::string getType() const override;
};


class NumericValue: public Value {
    double value;
public:
    NumericValue(double val): value{val} {}
    std::string toString() const override;
    std::string getType() const override;
    double asNumber() const;
};


class StringValue: public Value {
    std::string value;
    
public:
    StringValue(const std::string& val): value(val) {}
    std::string toString() const override;
    std::string getType() const override;
};


class NilValue: public Value {
public:
    NilValue() = default;
    std::string toString() const override;
    std::string getType() const override;
    std::vector<ValuePtr> toVector() const;
};

class SymbolValue: public Value {
    std::string name;
public:
    SymbolValue(const std::string& val): name(val) {}
    std::string toString() const override;
    std::string getType() const override;
    std::optional<std::string> asSymbol() const;
};


class PairValue: public Value {
    ValuePtr left;
    ValuePtr right;
public:
    PairValue(ValuePtr l, ValuePtr r): 
        left(std::move(l)), right(std::move(r)) {}
    std::string toString() const override;
    std::string getType() const override;
    std::vector<ValuePtr> toVector() const;
    ValuePtr getCar() const;
    ValuePtr getCdr() const;
};


class BuiltinProcValue : public Value {
    BuiltinFuncType* func;
public:
    BuiltinProcValue(BuiltinFuncType* f): func(f) {}
    std::string toString() const override;
    std::string getType() const override;
    ValuePtr apply(std::vector<ValuePtr> args);
};

class LambdaValue : public Value {
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> env;
public:
    LambdaValue(std::vector<std::string> p, 
                std::vector<ValuePtr> b,
                std::shared_ptr<EvalEnv> e): 
                params {p}, body{b}, env {std::move(e)} {}
    std::string toString() const override;
    std::string getType() const override;
};

#endif