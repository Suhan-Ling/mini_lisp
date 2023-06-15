#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "./error.h"
#include "./tokenizer.h"
#include "./value.h"
#include "./parser.h"
#include "./eval_env.h"
#include "./builtins.h"
#include "./forms.h"

#define RJSJ_TEST_NO_EXIT
#include "rjsj_test.hpp"

struct TestCtx {
    EnvPtr env = EvalEnv::createGlobal();
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env->eval(std::move(value));
        return result->toString();
    }
};

int main(int argc, char* argv[]) {
    // RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6, Lv7, Lv7Lib, Sicp);
    EnvPtr env = EvalEnv::createGlobal();
    bool repl = isatty(fileno(stdin));
    while (true) {
        try {
            if (repl) {
                std::cout << ">>> " ;
            }
            std::string line = "";
            std::string tail;
            int leftCnt = 0;
            int rightCnt = -1;
            while ((!std::cin.eof()) and (leftCnt != rightCnt)) {
                std::getline(std::cin, tail);
                line += tail.substr(0, tail.find(";"));
                leftCnt = std::count(line.begin(), line.end(), '(');
                rightCnt = std::count(line.begin(), line.end(), ')');
            }
            if (std::cin.eof()) {
                std::exit(0);
            }
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens)); 
            auto value = parser.parse();
            auto result = env->eval(std::move(value));  
            if (repl) {
                std::cout << result->toString() << std::endl;
            }
        } catch (std::runtime_error& e) {
            if (typeid(e) == (typeid(SyntaxError))) {
                std::cerr << "SyntaxError: " << e.what() << std::endl;
            } else if (typeid(e) == (typeid(LispError))) {
                std::cerr << "LispError: " << e.what() << std::endl;
            } else {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
}
