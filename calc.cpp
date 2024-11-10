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
    calc [--evaluate="EXPR"] [--tokens] [--ast]

Options:
    -e --evaluate="EXPR"  Evaluate expression non interactively. By default interactive.
    -t --tokens           Print tokens.
    -a --ast              Print Abstract Syntax Tree.
)";

void evaluate(std::string input, bool print_tokens, bool print_ast) {
    std::vector<Token> tokens;
    tokens = lex(input);

    if (print_tokens) {
        for (Token t : tokens) {
            fmt::println("{}", t);
        }
    }

    Parser parser{tokens};
    std::optional<AstPtr> ast = parser.parse();
    if (ast) {
        if (print_ast) {
            (*ast)->print();
        }
        fmt::println("{}", (*ast)->evaluate());
    }
}

int main(int argc, char** argv) {
    auto args = docopt::docopt(usage, {argv+1, argv+argc});
    bool print_tokens = args["--tokens"].asBool();
    bool print_ast = args["--ast"].asBool();

    if (args["--evaluate"]) {
        evaluate(args["--evaluate"].asString(), print_tokens, print_ast);
        return 0;
    }

    while (!std::cin.eof()) {
        fmt::print("> ");
        std::string input;
        std::getline(std::cin, input);
        try {
            evaluate(input, print_tokens, print_ast);
        } catch (std::exception& e) {
            fmt::println("Error: {}", e.what());
        }
    }
	fmt::println("");
}
