#include "./value.h"
#include "./error.h"
#include "./eval_env.h"
#include "./builtins.h"

#include <iostream>
#include <cmath>
#include <string>
#include <unordered_map>

const std::unordered_map<std::string, BuiltinFuncType*> BUILTIN_PROCS = {
    {"apply",       &__apply},	
    {"display",     &__display},	
    {"displayln",   &__displayln},	
    {"error",       &__error},	
    {"eval",        &__eval},	
    {"exit",        &__exit},	
    {"newline",     &__newline},	
    {"print",       &__print},

    {"atom?",       &__atom_},
    {"boolean?",    &__boolean_},
    {"integer?",    &__integer_},
    {"list?",       &__list_},
    {"number?",     &__number_},
    {"null?",       &__null_},
    {"pair?",       &__pair_},
    {"procedure?",  &__procedure_},
    {"string?",     &__string_},
    {"symbol?",     &__symbol_},

    {"append",      &__append},
    {"car",         &__car},
    {"cdr",         &__cdr},
    {"cons",        &__cons},
    {"length",      &__length},
    {"list",        &__list},
    {"map",         &__map},
    {"filter",      &__filter},
    {"reduce",      &__reduce},

    {"+",           &__add},
    {"-",           &__substract},
    {"*",           &__multiply}, 
    {"/",           &__divide},
    {"abs",         &__abs},
    {"expt",        &__expt},
    {"quotient",    &__quotient},
    {"modulo",      &__modulo},
    {"remainder",   &__remainder},

    {"eq?",         &__eq_},
    {"equal?",      &__equal_},
    {"not",         &__not},
    {"=",           &__equal},
    {">",           &__more},
    {"<",           &__less},
    {">=",          &__moreOrEqual},
    {"<=",          &__lessOrEqual},
    {"even?",       &__even_},
    {"odd?",        &__odd_},
    {"zero?",       &__zero_}
};

void argumentsLengthCheck(int len, int min, int max) {
    if (min != -1) {
        if (len < min) {
            throw LispError("Too few arguments: "
                            + std::to_string(len) + " < "
                            + std::to_string(min) + ".");
        }
    }
    if (max != -1) {
        if (len > max) {
            throw LispError("Too many arguments: "
                            + std::to_string(len) + " > "
                            + std::to_string(max) + ".");
        }
    }
}

ValuePtr __apply(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 2, 2);
    return params[0]->apply(params[1]->listToVector(), env);
}

ValuePtr __display(const std::vector<ValuePtr>& params, EvalEnv& env) {
    std::string str;
    for (auto i: params) {
        str = i->toString();
        if (i->isString()) {
            str.erase(str.begin());
            str.pop_back();
        }
        std::cout << str;
    }
    return std::make_shared<NilValue>();
}

ValuePtr __displayln(const std::vector<ValuePtr>& params, EvalEnv& env) {
    __display(params, env);
    __newline(params, env);
    return std::make_shared<NilValue>();
}

ValuePtr __error(const std::vector<ValuePtr>& params, EvalEnv& env) {
    int len = params.size();
    argumentsLengthCheck(len, -1, 1);
    if (len == 1) {
        throw LispError(params[0]->toString());
    } else {
        throw LispError("");
    }
}

ValuePtr __eval(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return env.eval(params[0]);
}

ValuePtr __exit(const std::vector<ValuePtr>& params, EvalEnv& env) {
    int len = params.size();
    argumentsLengthCheck(len, -1, 1);
    int code = 0;
    if (len == 1) {
        code = params[0]->asNumber();
    }
    exit(code);
}

ValuePtr __newline(const std::vector<ValuePtr>& params, EvalEnv& env) {
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr __print(const std::vector<ValuePtr>& params, EvalEnv& env) {
    for (const auto& i: params) {
        std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr __atom_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    ValuePtr p = params[0];
    bool b;
    b = p->isBool() or
        p->isNumber() or
        p->isString() or
        p->isSymbol() or
        p->isNil();
    return std::make_shared<BooleanValue>(b);
}

ValuePtr __boolean_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return std::make_shared<BooleanValue>(params[0]->isBool());
}

ValuePtr __integer_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    bool b = (params[0]->isNumber()) and 
             (params[0]->asNumber() == std::floor(params[0]->asNumber()));
    return std::make_shared<BooleanValue>(b);
}

ValuePtr __list_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return std::make_shared<BooleanValue>(params[0]->isList());
}

ValuePtr __number_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return std::make_shared<BooleanValue>(params[0]->isNumber());
}

ValuePtr __null_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return std::make_shared<BooleanValue>(params[0]->isNil());
}

ValuePtr __pair_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return std::make_shared<BooleanValue>(params[0]->isPair());
}

ValuePtr __procedure_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    ValuePtr p = params[0];
    return std::make_shared<BooleanValue>((p->isProc()) or (p->isLambda()));
}

ValuePtr __string_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return std::make_shared<BooleanValue>(params[0]->isString());
}

ValuePtr __symbol_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return std::make_shared<BooleanValue>(params[0]->isSymbol());
}

ValuePtr __append(const std::vector<ValuePtr>& params, EvalEnv& env) {
    std::vector<ValuePtr> elements, vec;
    for (auto i: params) {
        vec = i->listToVector();
        for (auto j: vec) {
            elements.push_back(j);
        }
    }
    return __list(elements, env);
}

ValuePtr __car(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return params[0]->getCar();
}

ValuePtr __cdr(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return params[0]->getCdr();
}

ValuePtr __cons(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 2, 2);
    return std::make_shared<PairValue>(params[0], params[1]);
}

ValuePtr __length(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return std::make_shared<NumericValue>(params[0]->listLength());
}

ValuePtr __list(const std::vector<ValuePtr>& params, EvalEnv& env) {
    ValuePtr cdr = std::make_shared<NilValue>();
    for (int i = params.size() - 1; i >= 0; i--) {
        cdr = std::make_shared<PairValue>(params[i], cdr);
    }
    return cdr;
}

ValuePtr __map(const std::vector<ValuePtr>& params, EvalEnv& env) {
    int len = params.size();
    argumentsLengthCheck(len, 2, 3);
    ValuePtr proc = params[0];
    std::vector<ValuePtr> args = params[1]->listToVector();
    int argc = 1;
    if (len == 3) {
        argc = params[2]->asNumber();
    }
    std::vector<ValuePtr> result;
    for (auto i = args.begin(); (i + argc - 1) != args.end(); i++) {
        result.push_back(proc->apply(std::vector<ValuePtr>(i, i + argc), env));
    }
    return __list(result, env);
}

ValuePtr __filter(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 2, 2);
    ValuePtr proc = params[0];
    std::vector<ValuePtr> args = params[1]->listToVector();
    std::vector<ValuePtr> result;
    for (auto i: args) {
        if (proc->apply(std::vector<ValuePtr>(1, i), env)->toString() != "#f") {
            result.push_back(i);
        }        
    }
    return __list(result, env);
}

ValuePtr __reduce(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 2, 2);
    ValuePtr proc = params[0];
    ValuePtr list = params[1];
    if (!list->isList()) {
        throw LispError("Reduce's second argument must be a list.");
    }
    int len = list->listLength();
    if (len > 1) {
        std::vector<ValuePtr> second {proc, list->getCdr()};
        std::vector<ValuePtr> args {list->getCar(), __reduce(second, env)};
        return proc->apply(args, env);
    } else if (len == 1) {
        return list->getCar();
    } else {    // len == 0
        throw LispError("Reduce list must has at least 1 element.");
    }
}

ValuePtr addAndMultiply(const std::vector<ValuePtr>& params, double result, 
                        binaryOperatorFuncType func) {
    for (const auto& i : params) {
        result = func(result, i->asNumber());
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr __add(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return addAndMultiply(params, 0, 
                            [](double x, double y) -> double {return x + y;});
}

ValuePtr __multiply(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return addAndMultiply(params, 1, 
                            [](double x, double y) -> double {return x * y;});
}

ValuePtr substractAndDivide(const std::vector<ValuePtr>& params, 
                            double defaultValue, binaryOperatorFuncType func) {
    int len = params.size();
    argumentsLengthCheck(len, 1, 2);
    if (len == 2) {
        double x = params[0]->asNumber();
        double y = params[1]->asNumber();
        return std::make_shared<NumericValue>(func(x, y));
    } else {    // len == 1
        double x = params[0]->asNumber();
        return std::make_shared<NumericValue>(func(defaultValue, x));
    }
}

ValuePtr __substract(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return substractAndDivide(params, 0, 
                            [](double x, double y) -> double {return x - y;});
}

ValuePtr __divide(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if ((params.size() == 2) and (params[1]->asNumber() == 0)) {
        throw LispError("Cannot divide 0.");
    }
    return substractAndDivide(params, 1, 
                            [](double x, double y) -> double {return x / y;});
}

ValuePtr __abs(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    double x = params[0]->asNumber();
    return std::make_shared<NumericValue>(fabs(x));
}

ValuePtr __expt(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 2, 2);
    double x = params[0]->asNumber();
    double y = params[1]->asNumber();
    return std::make_shared<NumericValue>(pow(x, y));
}

ValuePtr __quotient(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 2, 2);
    double x = params[0]->asNumber();
    double y = params[1]->asNumber();
    return std::make_shared<NumericValue>(int(x / y));
}

ValuePtr __modulo(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 2, 2);
    double x = params[0]->asNumber();
    double y = params[1]->asNumber();
    int z = floor(x / y);
    return std::make_shared<NumericValue>(x - y * z);
}

ValuePtr __remainder(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 2, 2);
    double x = params[0]->asNumber();
    double y = params[1]->asNumber();
    int z = int(x / y);
    return std::make_shared<NumericValue>(x - y * z);
}

ValuePtr __eq_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 2, 2);
    ValuePtr p = params[0];
    ValuePtr q = params[1];
    if (p->isBool()     or  q->isBool()     or
        p->isNumber()   or  q->isNumber()   or
        p->isSymbol()   or  q->isSymbol()   or
        p->isNil()      or  q->isNil()      ) {
        return __equal_(params, env);
    } else {
        bool b = (p == q);
        return std::make_shared<BooleanValue>(b);
    }
}

ValuePtr __equal_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 2, 2);
    bool b = (params[0]->toString().compare(params[1]->toString()));
    return std::make_shared<BooleanValue>(!b);
}

ValuePtr __not(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    bool b = (params[0]->toString() == "#f");
    return std::make_shared<BooleanValue>(b);
}

ValuePtr compare(const std::vector<ValuePtr>& params, compareFuncType* comp) {
    int len = params.size();
    argumentsLengthCheck(len, 2, 2);
    double x = params[0]->asNumber();
    double y = params[1]->asNumber();
    return std::make_shared<BooleanValue>(comp(x, y));
}

ValuePtr __equal(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return compare(params, [](double x, double y) -> bool {return x == y;});
}

ValuePtr __more(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return compare(params, [](double x, double y) -> bool {return x > y;});
}

ValuePtr __less(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return compare(params, [](double x, double y) -> bool {return x < y;});
}

ValuePtr __moreOrEqual(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return compare(params, [](double x, double y) -> bool {return x >= y;});
}

ValuePtr __lessOrEqual(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return compare(params, [](double x, double y) -> bool {return x <= y;});
}

ValuePtr __even_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    double x = params[0]->asNumber();
    return std::make_shared<BooleanValue>(isInt(x) && (int(x) % 2 == 0));
}

ValuePtr __odd_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    double x = params[0]->asNumber();
    return std::make_shared<BooleanValue>(isInt(x) && !(int(x) % 2 == 0));
}

ValuePtr __zero_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    argumentsLengthCheck(params.size(), 1, 1);
    return std::make_shared<BooleanValue>(params[0]->asNumber() == 0);
}