#include "./value.h"
#include "./error.h"
#include "./builtins.h"

#include <iostream>
#include <cmath>
#include <string>
#include <unordered_map>

const std::unordered_map<std::string, BuiltinFuncType*> BUILTIN_PROCS = {
    // {"apply",       &__apply},	
    // {"display",     &__display},	
    // {"displayln",   &__displayln},	
    // {"error",       &__error},	
    // {"eval",        &__eval},	
    // {"exit",        &__exit},	
    // {"newline",     &__newline},	
    {"print",       &__print},

    {"atom?",       &__atom_},
    {"boolean?",    &__boolean_},
    {"integer?",    &__integer_},
    // {"list?",       &__list_},
    {"number?",     &__number_},
    {"null?",       &__null_},
    {"pair?",       &__pair_},
    {"procedure?",  &__procedure_},
    {"string?",     &__string_},
    {"symbol?",     &__symbol_},

    // {"append",      &__append},
    // {"car",         &__car},
    // {"cdr",         &__cdr},
    // {"cons",        &__cons},
    // {"length",      &__length},
    // {"list",        &__list},
    // {"map",         &__map},
    // {"filter",      &__filter},
    // {"reduce",      &__reduce},

    {"+",           &__add},
    {"-",           &__substract},
    {"*",           &__multiply}, 
    {"/",           &__divide},
    // {"abs",         &__abs},
    // {"expt",        &__expt},
    // {"quotient",    &__quotient},
    // {"modulo",      &__modulo},
    // {"remainder",   &__remainder},

    // {"eq?",         &__eq_},
    // {"equal?",      &__equal_},
    // {"not",         &__not},
    {"=",           &__equal},
    {">",           &__more},
    {"<",           &__less},
    {">=",          &__moreOrEqual},
    {"<=",          &__lessOrEqual}
    // {"even?",       &__even_},
    // {"odd?",        &__odd_},
    // {"zero?",       &__zero_}
};

// ValuePtr __apply(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __display(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __displayln(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __error(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __eval(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __exit(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __newline(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

ValuePtr __print(const std::vector<ValuePtr>& params, EvalEnv& env) {
    for (const auto& i: params) {
        std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr __atom_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    bool b;
    b = (params[0]->getType() == "BooleanValue") or
        (params[0]->getType() == "NumericValue") or
        (params[0]->getType() == "StringValue") or
        (params[0]->getType() == "SymbolValue") or
        (params[0]->getType() == "NilValue");
    return std::make_shared<BooleanValue>(b);
}

ValuePtr __boolean_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "BooleanValue");
}

ValuePtr __integer_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    bool b;
    b = (params[0]->getType() == "NumericValue") and 
        (params[0]->asNumber() == std::floor(params[0]->asNumber()));
    return std::make_shared<BooleanValue>(b);
}

// ValuePtr __list_(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

ValuePtr __number_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "NumericValue");
}

ValuePtr __null_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "NilValue");
}

ValuePtr __pair_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "PairValue");
}

ValuePtr __procedure_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "BuiltinProcValue");
}

ValuePtr __string_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "StringValue");
}

ValuePtr __symbol_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return std::make_shared<BooleanValue>(params[0]->getType() == "SymbolValue");
}

// ValuePtr __append(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __car(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __cdr(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __cons(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __length(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __list(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __map(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __filter(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __reduce(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

ValuePtr addAndMultiply(const std::vector<ValuePtr>& params, double result, 
                        binaryOperatorFuncType func) {
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError(i->toString() + " is nut number.");
        }
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
    if (len == 2) {
        if (!params[0]->isNumber()) {
            throw LispError(params[0]->toString() + " is nut number.");
        } else if (!params[1]->isNumber()) {
            throw LispError(params[1]->toString() + " is nut number.");
        }
        return std::make_shared<NumericValue>(func(params[0]->asNumber(), params[1]->asNumber()));
    } else if (len == 1) {
        if (!params[0]->isNumber()) {
            throw LispError(params[0]->toString() + " is nut number.");
        }
        return std::make_shared<NumericValue>(func(defaultValue, params[0]->asNumber()));
    } else if (len > 2) {
        throw LispError("Too many arguments: " + std::to_string(len) + " > 2.");
    } else {    // len == 0
        throw LispError("Too few arguments: 0 < 1.");
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

// ValuePtr __abs(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __expt(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __quotient(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __modulo(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __remainder(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __eq_(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __equal_(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __not(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

ValuePtr compare(const std::vector<ValuePtr>& params, compareFuncType* comp) {
    int len = params.size();
    if (len < 2) {
        throw LispError("Too few arguments: " + std::to_string(len) + " < 2.");
    }
    auto x = params[0];
    auto y = params[1];
    if (!x->isNumber()) {
        throw LispError(x->toString() + "is not number.");
    }
    if (!y->isNumber()) {
        throw LispError(y->toString() + "is not number.");
    }
    return std::make_shared<BooleanValue>(comp(x->asNumber(), y->asNumber()));
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

// ValuePtr __even_(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __odd_(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }

// ValuePtr __zero_(const std::vector<ValuePtr>& params, EvalEnv& env) {

// }