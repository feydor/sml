#include "object.h"
#include "libstr.h"

namespace Obj {
	String::String(const std::string& val) : val_(val)
	{
		methods_.emplace("len", new Lib::String::len());
		methods_.emplace("at", new Lib::String::at());
	};

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

	std::shared_ptr<Object>
	String::get(size_t i) const
	{
		std::string char_to_str(1, val_[i]);
		return std::make_shared<String>(char_to_str);
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
