#include "env.h"
#include <vector>
#include <stdexcept>

namespace Env {
	// start with an Environment for globals
	auto Envs = std::vector<Environment>({Environment()});

	void
	push_back()
	{
		Envs.emplace_back();
	}

	void
	pop()
	{
		Envs.pop_back();
	}

	bool
	var_exists(Environment &env, const std::string& name)
	{
		return env.vars.find(name) != env.vars.end();
	}

	// vars can only be set at the current Env (Envs.back())
	void
	set_var(const std::string& name, Obj::Object* val)
	{
		auto entry = Envs.back().vars.find(name);
		if (entry != Envs.back().vars.end())
			entry->second = val;
		else
			Envs.back().vars[name] = val;
	}

	// starts at most recent env/scope and searches in reverse order
	Obj::Object*
	get_var(const std::string& name)
	{
		// iterate from back to front of vector, calling var_exists on each Environment
		for (auto itr = Envs.rbegin(); itr != Envs.rend(); ++itr) {
			if (var_exists((*itr), name))
				return (*itr).vars[name];
		}
		return new Obj::String("<fn " + name + ">");
	}

	std::string
	to_str()
	{
		std::string out("");
		int i = 0;
		for (const auto& env : Envs) {
			out += "----------ENV" + std::to_string(i) + "---------\n";
			out += "size: " + std::to_string(env.vars.size()) + "\n";
			i++;
			for (const auto& var : env.vars)
				out += var.first + " = " + var.second->to_str() + "\n";
		}
		return out;
	}

}
