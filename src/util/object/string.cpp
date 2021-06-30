#include "object.h"

namespace Obj {

	Object_t
	String::type() const
	{
		return Object_t::STR;
	}

	std::string
	String::to_str() const
	{
		return str();
	}

	bool
	String::is_truthy() const
	{
		return false;
	}

	std::string
	String::str() const
	{
		return val_;
	}

	size_t
	String::size() const
	{
		return str().size();
	}

	bool
	String::compare(const String& other) const
	{
		if (size() != other.size()) return false;
		for (size_t i = 0; i < size(); ++i)
			if (str()[i] != other.str()[i])
				return false;
		return true;
	}

};
