#include "object.h"

namespace Obj {

	Object_t
	Array::type() const
	{
		return Object_t::ARR;
	}

	std::string
	Array::to_str() const
	{
		return get(0)->to_str();
	}

	bool
	Array::is_truthy() const
	{
		return true; // TODO: is an array identifier truthy?
	}

	Object*
	Array::get(size_t i) const
	{
		return objects_[i];
	}

	void
	Array::put(size_t i, Object* obj)
	{
		objects_[i] = obj;
	}

	void
	Array::push_back(Object* obj)
	{
		objects_.push_back(obj);
	}

	size_t
	Array::size() const
	{
		return objects_.size();
	}

	bool
	Array::compare(const Array* other) const
	{
		if (size() != other->size()) return false;
		for (size_t i = 0; i < size(); ++i)
			if (get(i) != other->get(i))
				return false;
		return true;
	}

	Array::~Array()
	{
		for (auto& obj : objects_)
			delete obj;
	}

};
