#ifndef SMOL_FN_H
#define SMOL_FN_H
#include "value.h"
#include "stmt.h"
#include <string>
#include <vector>

class Fn {
	public:
	virtual std::string name() = 0;
	virtual Val invoke(std::vector<Val> args) = 0;
	void check_args(std::vector<Val> args, int count);
	virtual ~Fn() = default;
};

class UserFn : public Fn {
	public:
	UserFn(std::string name)
		: name_(name) {};
	void set_body(Ast::Stmt* body);
	void add_argname(std::string name);
	std::string name() override;
	Val invoke(std::vector<Val> args) override;
	~UserFn() override;

	private:
	std::string name_;
	std::vector<std::string> argnames_;
	Ast::Stmt* body_;
};

#endif