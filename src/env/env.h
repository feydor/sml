#ifndef SMOL_ENV_H
#define SMOL_ENV_H
#include "object.h"
#include <string>
#include <unordered_map>

namespace Env {
	void push_back();
	void pop();
	bool var_exists(const std::string& name);
	void set_var(const std::string& name, Obj::Object* val);
	Obj::Object* get_var(const std::string& name);
	std::string to_str();
}

struct Environment {
    std::unordered_map<std::string, Obj::Object*> vars;
};

#endif
