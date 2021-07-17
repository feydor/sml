#ifndef SMOL_ENV_H
#define SMOL_ENV_H
#include <string>
#include <unordered_map>
#include <memory>

namespace Obj {class Object; }

namespace Env {
	void push_back();
	void pop();
	bool var_exists(const std::string& name);
	void set_var(const std::string& name, std::shared_ptr<Obj::Object> val);
	std::shared_ptr<Obj::Object> get_var(const std::string& name);
	std::string to_str();
}

struct Environment {
    std::unordered_map<std::string, std::shared_ptr<Obj::Object>> vars;
};

#endif
