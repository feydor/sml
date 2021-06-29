#include "object.h"

namespace Smol {

	Object_t
	Bool::type() const
	{
		return Object_t::BOOL;
	}

	std::string
	Bool::to_str() const
	{
		return std::to_string(get());
	}

	bool
	Bool::is_truthy() const
	{
		return get();
	}

	std::unique_ptr<Object>
	Bool::operator+(const Object& other) const
	{
		return std::make_unique<Bool>(get() && other.get());
	}

	bool
	Bool::get() const
	{
		return val_;
	}

};
