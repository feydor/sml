#include "env.h"
#include <vector>
#include <stdexcept>

namespace Env {

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

	void
	set_var(const std::string& name, Val val)
	{
		Envs.back().vars[name] = std::move(val);
	}

	// starts at most recent env/scope and searches in reverse order
	Val
	get_var(const std::string& name)
	{
		// iterate from back to front of vector, calling var_exists on each Environment
		for (auto itr = Envs.rbegin(); itr != Envs.rend(); ++itr) {
			if (var_exists((*itr), name))
				return (*itr).vars[name];
		}
		throw new std::runtime_error("variable '" + name + "' does not exist.");			
	}

}
