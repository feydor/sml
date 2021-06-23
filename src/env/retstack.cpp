#include "retstack.h"

auto RetStack::rstack = std::vector<Val>();

void
RetStack::push_back(Val v)
{
	rstack.push_back(v);
}

Val
RetStack::pop()
{
	// if empty, this means no return statement, return NIL
	// which is discarded
	if (is_empty())
		return Val();
	auto top = rstack.back();
	rstack.pop_back();
	return top;
}

std::string
RetStack::to_str()
{
	std::string res("");
	res += "\n-------RetStack-------\n";
	res += "size: " + std::to_string(rstack.size()) + "\n";
	for (auto v : rstack) {
		res += v.to_str() + "\n";
	}
	return res;
}

bool
RetStack::is_empty()
{
	return rstack.empty();
}
