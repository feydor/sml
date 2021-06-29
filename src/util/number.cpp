#include "object.h"

namespace Smol {

	Object_t
	Number::type() const
	{
		return Object_t::NUM;
	}

	std::string
	Number::to_str() const
	{
		return std::to_string(get());
	}

	bool
	Number::is_truthy() const
	{
		return get() == 0 ? false : true;
	}

	std::unique_ptr<Object>
	Number::operator+(const Object& other) const
	{
		return std::make_unique<Number>(get() + other.get());
	}

	double
	Number::get() const
	{
		return val_;
	}

};
