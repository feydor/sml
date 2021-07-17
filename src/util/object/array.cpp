#include "object.h"
#include "libarr.h"

namespace Obj {
	Array::Array(const std::vector<std::shared_ptr<Object>>& objects)
		: objects_(objects)
	{
		methods_.insert(std::make_pair("put", std::make_shared<Lib::Array::put>()));
	};

	Object_t
	Array::type() const
	{
		return Object_t::ARR;
	}

	std::string
	Array::to_str() const
	{
		std::string out("[");
		for (const auto& obj : objects_)
			out += "" + obj->to_str() + " ";
		out.pop_back();
		out += "]";
		return out;
	}

	bool
	Array::is_truthy() const
	{
		return true; // TODO: is an array identifier truthy?
	}

	std::shared_ptr<Object>
	Array::get(size_t i) const
	{
		return objects_[i];
	}

	void
	Array::put(size_t i, std::shared_ptr<Object> obj)
	{
		objects_[i] = std::move(obj);
	}

	void
	Array::push_back(std::shared_ptr<Object> obj)
	{
		objects_.push_back(std::move(obj));
	}

	size_t
	Array::size() const
	{
		return objects_.size();
	}

	bool
	Array::compare(const Array& other) const
	{
		if (size() != other.size()) return false;
		for (size_t i = 0; i < size(); ++i)
			if (get(i)->to_str() != other.get(i)->to_str())
				return false;
		return true;
	}

	std::shared_ptr<Object>
	Array::begin() const
	{
		return objects_[0];
	}

	Array::~Array()
	{
		/*
		for (auto& obj : objects_)
			delete obj;
		*/
	}

};
