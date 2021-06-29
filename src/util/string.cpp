#include "object.h"

namespace Smol {

	Object_t
	String::type() const
	{
		return Object_t::STR;
	}

	std::string
	String::to_str() const
	{
		return this->get();
	}

	bool
	String::is_truthy() const
	{
		return false;
	}

	std::unique_ptr<Object>
	String::operator+(const Object& other) const
	{
		return std::make_unique<String>(get() + other.get());
	}

	std::string
	String::get() const
	{
		return val_;
	}

};
