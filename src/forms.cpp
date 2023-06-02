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
    {"define",  defineForm},
    {"quote",   quoteForm},
    {"if",      ifForm},
    {"and",     andForm},
    {"or",      orForm},
    {"lambda",  lambdaForm}
};

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (auto name = args[0]->asSymbol()) {      // variable
        env.defineBinding(*name, args[1]);
            std::cout << env.lookupBinding(*name)->toString() << std::endl;
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

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    return args[0];
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (env.eval(args[0])->toString() == "#f") {
        return env.eval(args[2]);
    } else {
        return env.eval(args[1]);
    }
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

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    std::vector<std::string> params;
    for (auto i: args[0]->toVector()) {
        params.push_back(i->toString());
    }
    std::vector<ValuePtr> body = args;
    body.erase(body.begin());
    return std::make_shared<LambdaValue>(params, body, env.shared_from_this());
}