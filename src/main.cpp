#include <iostream>
#include <string>
#include <typeinfo>

#include "./token.h"
#include "./tokenizer.h"
#include "./value.h"
#include "./parser.h"

int main() {
    while (true) {
        try {
            std::cout << ">>> " ;
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                std::exit(0);
            }
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));               // TokenPtr 不支持复制
            auto value = parser.parse();
            std::cout << value->toString() << std::endl;    // 输出外部表示
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
