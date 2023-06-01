#include "./value.h"
#include "./error.h"
#include "./eval_env.h"
#include "./forms.h"

#include <unordered_map>
#include <iostream>

std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS {
    {"define", defineForm},
    {"quote", quoteForm},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm},
};

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (auto name = args[0]->asSymbol()) {
        env.addSymbol(*name, args[1]);
        return std::make_shared<NilValue>();
    } else {
        throw LispError("Malformed define.");
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
    return args[0];
}