#include <fmt/ranges.h>
#include <string>
#include <iostream>
#include <vector>
#include <libcalc/lex.hpp>
#include <libcalc/parse.hpp>

int main() {
    while (!std::cin.eof()) {
        fmt::print("> ");
        std::string input;
        std::getline(std::cin, input);

		std::vector<Token> tokens;
		try {
			 tokens = lex(input);
		} catch (const std::invalid_argument& e) {
			fmt::println("Error in Lexer: {}", e.what());
		}

		if (!tokens.empty()) {
			// fmt::println("Tokens:");
			// for (Token t : tokens) {
			// 	fmt::println("{} '{}'", t.type, t.text);
			// }

			std::unique_ptr<Ast> tree = parse(tokens.begin(), tokens.end());
			fmt::println("{}", tree->evaluate());
		}
    }
	fmt::println("");
}
