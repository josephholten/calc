#include <fmt/ranges.h>
#include <string>
#include <iostream>
#include <vector>
#include <libcalc/lex.hpp>
#include <libcalc/parse.hpp>
#include <docopt.cpp/docopt.h>

static const char usage[] =
R"(calc.
Usage:
    calc [--evaluate="EXPR"] [--tokens]

Options:
    -e --evaluate="EXPR"  Evaluate expression non interactively. By default interactive.
    -t --tokens           Print tokens.
)";

void evaluate(std::string input, bool print_tokens) {
    std::vector<Token> tokens;
    tokens = lex(input);

    if (!tokens.empty()) {
        if (print_tokens) {
            fmt::println(" -- Tokens:");
            for (Token t : tokens) {
                fmt::println(" -- {} '{}'", t.type, t.text);
            }
        }

        std::unique_ptr<Ast> tree = parse(tokens.begin(), tokens.end());
        fmt::println("{}", tree->evaluate());
    }
}

int main(int argc, char** argv) {
    auto args = docopt::docopt(usage, {argv+1, argv+argc});

    if (args["--evaluate"]) {
        evaluate(args["--evaluate"].asString(), args["--tokens"].asBool());
        return 0;
    }

    while (!std::cin.eof()) {
        fmt::print("> ");
        std::string input;
        std::getline(std::cin, input);
        try {
            evaluate(input, args["--tokens"].asBool());
        } catch (std::exception& e) {
            fmt::println("Error: {}", e.what());
        }
    }
	fmt::println("");
}
