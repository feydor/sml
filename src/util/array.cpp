#include "object.h"

namespace Smol {

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

	std::unique_ptr<Object>
	Array::operator+(const Object& other) const
	{
		return nullptr;// TODO: std::make_unique<Number>(get() + other.get());
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

	Array::~Array()
	{
		for (auto& obj : objects_)
			delete obj;
	}

};
