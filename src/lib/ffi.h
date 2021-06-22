/* ffi.h - foreign function interface (FFI) base and built-in subclasses */
#ifndef SMOL_FFI_H
#define SMOL_FFI_H
#include "value.h"
#include "stmt.h"
#include <string>
#include <vector>

class FFInterface {
	public:
	virtual std::string name() = 0;
	virtual Val invoke(std::vector<Val> args) = 0;
	void check_nargs(std::vector<Val> args, int count);
	virtual ~FFInterface() = default;
};

class UserFn : public FFInterface {
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
