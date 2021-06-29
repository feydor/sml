#include "object.h"

namespace Obj {

	Object_t
	Bool::type() const
	{
		return Object_t::BOOL;
	}

	std::string
	Bool::to_str() const
	{
		return bol() ? "true" : "false";
	}

	bool
	Bool::is_truthy() const
	{
		return bol();
	}

	bool
	Bool::bol() const
	{
		return val_;
	}

};
