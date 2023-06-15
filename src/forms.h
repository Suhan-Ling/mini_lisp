#ifndef FORMS_H
#define FORMS_H

#include "./value.h"
#include "./eval_env.h"

#include <unordered_map>

extern const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS;

SpecialFormType defineForm;
SpecialFormType lambdaForm;
SpecialFormType quoteForm;
SpecialFormType quasiquoteForm;
SpecialFormType unquoteForm;
SpecialFormType ifForm;
SpecialFormType condForm;
SpecialFormType andForm;
SpecialFormType orForm;
SpecialFormType beginForm;
SpecialFormType letForm;

#endif
