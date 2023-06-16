#include "./value.h"
#include "./error.h"
#include "./eval_env.h"
#include "./forms.h"

#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS {
    {"define",      defineForm},
    {"lambda",      lambdaForm},
    {"quasiquote",  quasiquoteForm},
    {"unquote",     unquoteForm},
    {"quote",       quoteForm},
    {"if",          ifForm},
    {"cond",        condForm},
    {"and",         andForm},
    {"or",          orForm},
    {"begin",       beginForm},
    {"let",         letForm}
};

void operandsLengthCheck(int len, int min, int max) {
    if (min != -1) {
        if (len < min) {
            throw LispError("Too few operands: "
                            + std::to_string(len) + " < "
                            + std::to_string(min) + ".");
        }
    }
    if (max != -1) {
        if (len > max) {
            throw LispError("Too many operands: "
                            + std::to_string(len) + " > "
                            + std::to_string(max) + ".");
        }
    }
}

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    operandsLengthCheck(args.size(), 2, -1);
    if (auto name = args[0]->asSymbol()) {      // variable
        env.defineBinding(*name, env.eval(args[1]));
        return std::make_shared<NilValue>();
    } else if (args[0]->isPair()) {             // lambda
        std::string lambdaName = args[0]->getCar()->toString();
        std::vector<std::string> params;
        for (auto i: args[0]->getCdr()->toVector()) {
            params.push_back(i->toString());
        }
        std::vector<ValuePtr> body = args;
        body.erase(body.begin());
        auto lambda = std::make_shared<LambdaValue>(params, body, env.shared_from_this());
        env.defineBinding(lambdaName, lambda);
        return std::make_shared<NilValue>();
    } else {
        throw LispError("Malformed define form: " + args[0]->toString() + ".");
    }
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    operandsLengthCheck(args.size(), 2, -1);
    std::vector<std::string> params;
    for (auto i: args[0]->toVector()) {
        params.push_back(i->toString());
    }
    std::vector<ValuePtr> body = args;
    body.erase(body.begin());
    return std::make_shared<LambdaValue>(params, body, env.shared_from_this());
}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    operandsLengthCheck(args.size(), 1, 1);
    return args[0];
}

ValuePtr quasiquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    operandsLengthCheck(args.size(), 1, 1);
    ValuePtr result = args[0];
    ValuePtr car, cdr;
    if (result->isPair()) {
        car = result->getCar();
        cdr = result->getCdr();
        if (car->toString() == "unquote") {
            result = env.eval(cdr->getCar());
        } else {
            result = std::make_shared<PairValue>(
                quasiquoteForm(std::vector<ValuePtr>(1, car), env),
                quasiquoteForm(std::vector<ValuePtr>(1, cdr), env)
            );
        }
    }
    return result;
}

ValuePtr unquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    operandsLengthCheck(args.size(), 1, 1);
    return env.eval(args[0]);
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    operandsLengthCheck(args.size(), 2, 3);
    if (env.eval(args[0])->toString() == "#f") {
        return env.eval(args[2]);
    } else {
        return env.eval(args[1]);
    }
}

ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    ValuePtr result = std::make_shared<NilValue>();
    std::vector<ValuePtr> list;
    for (auto i: args) {
        list = i->asList();
        operandsLengthCheck(list.size(), 1, -1);
        if (list[0]->toString() == "else") {
            if (i != *args.rbegin()) {
                throw LispError("Else clause must be the last one.");
            }
            list.erase(list.begin());
            for (auto j: list) {
                result = env.eval(j);
            }
            break;
        }
        result = env.eval(list[0]);
        if (result->toString() != "#f") {
            list.erase(list.begin());
            for (auto j: list) {
                result = env.eval(j);
            }
            break;
        }
    }
    return result;
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    int len = args.size();
    if (len == 0) {
        return std::make_shared<BooleanValue>(true);
    }
    for (auto i: args) {
        if (env.eval(i)->toString() == "#f") {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return env.eval(args[len - 1]);
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    for (auto i: args) {
        auto v = env.eval(i);
        if (v->toString() != "#f") {
            return v;
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    ValuePtr result = std::make_shared<NilValue>();
    for (auto i: args) {
        result = env.eval(i);
    }
    return result;
}

ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    int len = args.size();
    operandsLengthCheck(len, 2, -1);
    EnvPtr envChild = EvalEnv::createChild(env.shared_from_this());
    std::vector<ValuePtr> params = args[0]->asList();
    for (auto i: params) {
        std::vector<ValuePtr> bond = i->toVector();
        operandsLengthCheck(bond.size(), 2, -1);
        if (auto name = bond[0]->asSymbol()) {
            envChild->defineBinding(*name, env.eval(bond[1]));
        } else {
            defineForm(bond, *envChild);
        }
    }    
    ValuePtr result = std::make_shared<NilValue>();    
    for (auto j: args) {
        if (j != args[0]) {
            result = envChild->eval(j);
        }
    }
    return result;
}
