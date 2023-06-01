#include "./value.h"
#include "./error.h"
#include "./eval_env.h"
#include "./forms.h"

#include <unordered_map>

const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS {
    {"define", defineForm}
};

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (auto name = args[0]->asSymbol()) {
        env.addSymbol(*name, args[1]);
        return std::make_shared<NilValue>();
    } else {
        throw LispError("Malformed define.");
    }
}