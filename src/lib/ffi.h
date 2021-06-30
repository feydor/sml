/* ffi.h - foreign function interface (FFI) base and built-in subclasses */
#ifndef SMOL_FFI_H
#define SMOL_FFI_H
#include "object.h"
#include "stmt.h"
#include <string>
#include <vector>

class FFInterface {
	public:
	virtual std::string name() = 0;
	virtual Obj::Object* invoke(std::vector<Obj::Object*> args) = 0;
	void check_nargs(const std::vector<Obj::Object*> &args, int count);
	virtual ~FFInterface() = default;
};

class UserFn : public FFInterface {
	public:
	explicit UserFn(std::string name)
		: name_(std::move(name)) {};
	void set_body(Ast::Stmt* body);
	void add_argname(std::string name);
	std::string name() override;
	Obj::Object* invoke(std::vector<Obj::Object*> args) override;
	~UserFn() override;

	private:
	std::string name_;
	std::vector<std::string> argnames_;
	Ast::Stmt* body_ = nullptr;
};

#endif
