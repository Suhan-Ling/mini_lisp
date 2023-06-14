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

void lenCheck(int len, int min, int max) {
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
    lenCheck(params.size(), 1, -1);
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
    lenCheck(params.size(), 1, -1);
    return std::make_shared<BooleanValue>(params[0]->isBool());
}

ValuePtr __integer_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 1, -1);
    bool b = (params[0]->isNumber()) and 
             (params[0]->asNumber() == std::floor(params[0]->asNumber()));
    return std::make_shared<BooleanValue>(b);
}

// ValuePtr __list_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    // lenCheck(params.size(), 1, -1);

// }

ValuePtr __number_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 1, -1);
    return std::make_shared<BooleanValue>(params[0]->isNumber());
}

ValuePtr __null_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 1, -1);
    return std::make_shared<BooleanValue>(params[0]->isNil());
}

ValuePtr __pair_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 1, -1);
    return std::make_shared<BooleanValue>(params[0]->isPair());
}

ValuePtr __procedure_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 1, -1);
    ValuePtr p = params[0];
    return std::make_shared<BooleanValue>((p->isProc()) or (p->isLambda()));
}

ValuePtr __string_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 1, -1);
    return std::make_shared<BooleanValue>(params[0]->isString());
}

ValuePtr __symbol_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 1, -1);
    return std::make_shared<BooleanValue>(params[0]->isSymbol());
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
    lenCheck(len, 1, 2);
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
    lenCheck(params.size(), 1, -1);
    double x = params[0]->asNumber();
    return std::make_shared<NumericValue>(fabs(x));
}

ValuePtr __expt(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 2, -1);
    double x = params[0]->asNumber();
    double y = params[1]->asNumber();
    return std::make_shared<NumericValue>(pow(x, y));
}

ValuePtr __quotient(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 2, -1);
    double x = params[0]->asNumber();
    double y = params[1]->asNumber();
    return std::make_shared<NumericValue>(int(x / y));
}

ValuePtr __modulo(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 2, -1);
    double x = params[0]->asNumber();
    double y = params[1]->asNumber();
    int z = floor(x / y);
    return std::make_shared<NumericValue>(x - y * z);
}

ValuePtr __remainder(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 2, -1);
    double x = params[0]->asNumber();
    double y = params[1]->asNumber();
    int z = int(x / y);
    return std::make_shared<NumericValue>(x - y * z);
}

ValuePtr __eq_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 2, -1);
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
    lenCheck(params.size(), 2, -1);
    bool b = (params[0]->toString().compare(params[1]->toString()));
    return std::make_shared<BooleanValue>(!b);
}

ValuePtr __not(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 1, -1);
    bool b = (params[0]->toString() == "#f");
    return std::make_shared<BooleanValue>(b);
}

ValuePtr compare(const std::vector<ValuePtr>& params, compareFuncType* comp) {
    lenCheck(params.size(), 2, -1);
    int len = params.size();
    if (len < 2) {
        throw LispError("Too few arguments: " + std::to_string(len) + " < 2.");
    }
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
    lenCheck(params.size(), 1, -1);
    double x = params[0]->asNumber();
    return std::make_shared<BooleanValue>(isInt(x) && (int(x) % 2 == 0));
}

ValuePtr __odd_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 1, -1);
    double x = params[0]->asNumber();
    return std::make_shared<BooleanValue>(isInt(x) && !(int(x) % 2 == 0));
}

ValuePtr __zero_(const std::vector<ValuePtr>& params, EvalEnv& env) {
    lenCheck(params.size(), 1, -1);
    return std::make_shared<BooleanValue>(params[0]->asNumber() == 0);
}