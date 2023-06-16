#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <memory>
#include <typeinfo>
#include <vector>
#include <optional>

class Value;
using ValuePtr = std::shared_ptr<Value>;

class EvalEnv;
using EnvPtr = std::shared_ptr<EvalEnv>;

using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);
using SpecialFormType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);

bool isInt(double value);

class Value {
public:
    virtual ~Value() = default;
    virtual std::string toString() const = 0;
    bool isBool() const;
    bool isNumber() const;
    bool isString() const;
    bool isNil() const;
    bool isSymbol() const;
    bool isPair() const;
    bool isProc() const;
    bool isLambda() const;
    bool isSelfEvaluating() const;
    virtual bool isList() const;
    virtual bool isMatrix() const;
    virtual int length() const;
    virtual ValuePtr shape() const;
    virtual void checkRange(int x, int y) const;
    virtual int rowNum() const;
    virtual int colNum() const;
    virtual int sub(int x, int y) const;
    virtual ValuePtr minor(int x, int y) const;
    virtual int det() const;
    virtual std::vector<ValuePtr> toVector() const;
    virtual std::vector<ValuePtr> asList() const;
    virtual std::vector<ValuePtr> asRow() const;
    virtual std::optional<std::string> asSymbol() const;
    virtual double asNumber() const;
    virtual ValuePtr getCar() const;
    virtual ValuePtr getCdr() const;
    virtual std::string getType() const = 0;
    virtual ValuePtr apply(const std::vector<ValuePtr>& args, EvalEnv& env) const;
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
    ValuePtr car;
    ValuePtr cdr;
public:
    PairValue(ValuePtr l, ValuePtr r): 
        car(std::move(l)), cdr(std::move(r)) {}
    bool isList() const;
    bool isMatrix() const;
    std::string toString() const override;
    std::string getType() const override;
    int length() const;
    ValuePtr shape() const;
    void checkRange(int x, int y) const;
    int rowNum() const;
    int colNum() const;
    int sub(int x, int y) const;
    ValuePtr minor(int x, int y) const;
    int det() const;
    std::vector<ValuePtr> toVector() const;
    std::vector<ValuePtr> asList() const;
    std::vector<ValuePtr> asRow() const;
    ValuePtr getCar() const;
    ValuePtr getCdr() const;
};

class BuiltinProcValue : public Value {
    BuiltinFuncType* func;
public:
    BuiltinProcValue(BuiltinFuncType* f): func(f) {}
    std::string toString() const override;
    std::string getType() const override;
    ValuePtr apply(const std::vector<ValuePtr>& args, EvalEnv& env) const;
};

class LambdaValue : public Value {
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    EnvPtr env;
public:
    LambdaValue(std::vector<std::string> p, 
                std::vector<ValuePtr> b,
                EnvPtr e): 
                params {p}, body{b}, env {std::move(e)} {}
    std::string toString() const override;
    std::string getType() const override;
    ValuePtr apply(const std::vector<ValuePtr>& args, EvalEnv& env) const;
};

#endif
