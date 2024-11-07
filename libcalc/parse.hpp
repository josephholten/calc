#ifndef LIBCALC_PARSE_HPP
#define LIBCALC_PARSE_HPP

#include <fmt/ranges.h>
#include <vector>
#include <libcalc/lex.hpp>

enum class OperatorType {
    Plus,
    Minus,
    Mult,
    Div,
    Exp,
	None,
};

template <>
struct fmt::formatter<OperatorType> : formatter<string_view> {
	format_context::iterator format(OperatorType op, format_context& ctx) const;
};

class Ast {
public:
	virtual double evaluate() = 0;
};

class OperatorNode : public Ast {
public:
	OperatorNode(std::unique_ptr<Ast> _left, OperatorType _op, std::unique_ptr<Ast> _right) 
		: left{std::move(_left)}, op{_op}, right{std::move(_right)}
	{ }

	double evaluate() override;

private:
    std::unique_ptr<Ast> left;
	OperatorType op;
    std::unique_ptr<Ast> right;
};

class NumberNode : public Ast {
public:
	NumberNode(double _x)
		: x{_x}
	{ }

	double evaluate() override {
		return x;
	}

private:
	double x;
};

std::unique_ptr<Ast> parse(std::vector<Token>::const_iterator start, std::vector<Token>::const_iterator end);

#endif /* LIBCALC_PARSE_HPP */
