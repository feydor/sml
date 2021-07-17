/* ffi.h - foreign function interface (FFI) base and built-in subclasses */
#ifndef SMOL_FFI_H
#define SMOL_FFI_H
// #include "object.h"
// #include "stmt.h"
#include <string>
#include <vector>
#include <memory>

namespace Obj {
	class Object;
}
namespace Ast { class Stmt; }

class FFInterface {
	public:
	virtual std::string name() = 0;
	virtual std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) = 0;
	void check_nargs(const std::vector<std::shared_ptr<Obj::Object>> &args, int count);
	virtual ~FFInterface() = default;
};

class UserFn : public FFInterface {
	public:
	explicit UserFn(std::string name);
	void set_body(std::unique_ptr<Ast::Stmt> body);
	void add_argname(std::string name);
	std::string name() override;
	std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
	~UserFn() override;

	private:
	std::string name_;
	std::vector<std::string> argnames_;
	std::unique_ptr<Ast::Stmt> body_;
};

#endif
