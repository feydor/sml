#include "object.h"
#include "ffi.h"
#include <cmath>  // for std::fmod
#include <iostream>

namespace Obj {
	
	// Binary operators
	std::unique_ptr<Object>
	Object::operator+(const Object& other) const
	{
		if (type() == Object_t::NUM) {
			auto a = dynamic_cast<const Number*>(this);
			if (other.type() == Object_t::NUM) {
				auto b = dynamic_cast<const Number*>(&other);
				return std::make_unique<Number>(a->num() + b->num());
			}
			else if (other.type() == Object_t::STR) {
				auto b = dynamic_cast<const String*>(&other);
				return std::make_unique<String>(a->to_str() + b->str());
			}
		}
		else if (type() == Object_t::STR) {
			auto a = dynamic_cast<const String*>(this);
			if (other.type() == Object_t::STR) {
				auto b = dynamic_cast<const String*>(&other);
				return std::make_unique<String>(a->str() + b->str());
			}
			else if (other.type() == Object_t::NUM) {
				auto b = dynamic_cast<const Number*>(&other);
				return std::make_unique<String>(a->str() + b->to_str());
			}
		}
		else if (type() == Object_t::BOOL && other.type() == Object_t::BOOL) {
			auto a = dynamic_cast<const Bool*>(this);
			auto b = dynamic_cast<const Bool*>(&other);
			return std::make_unique<Bool>(a->bol() && b->bol());
		}
		throw type_error("Cannot add", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator-(const Object& other) const
	{
		if (type() == Object_t::NUM && other.type() == Object_t::NUM) {
			auto a = dynamic_cast<const Number*>(this);
			auto b = dynamic_cast<const Number*>(&other);
			return std::make_unique<Number>(a->num() - b->num());
		}
		throw type_error("Cannot subtract", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator*(const Object& other) const
	{
		if (type() == Object_t::NUM && other.type() == Object_t::NUM) {
			auto a = dynamic_cast<const Number*>(this);
			auto b = dynamic_cast<const Number*>(&other);
			return std::make_unique<Number>(a->num() * b->num());
		}
		throw type_error("Cannot multiply", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator/(const Object& other) const
	{
		if (type() == Object_t::NUM && other.type() == Object_t::NUM) {
			auto a = dynamic_cast<const Number*>(this);
			auto b = dynamic_cast<const Number*>(&other);
			return std::make_unique<Number>(a->num() / b->num());
		}
		throw type_error("Cannot divide", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator%(const Object& other) const
	{
		if (type() == Object_t::NUM && other.type() == Object_t::NUM) {
			auto a = dynamic_cast<const Number*>(this);
			auto b = dynamic_cast<const Number*>(&other);
			return std::make_unique<Number>(std::fmod(a->num(), b->num()));
		}
		throw type_error("Cannot mod", type(), other.type());
	}

	std::shared_ptr<Object>
	Object::operator[](const Object& other) const
	{
		if (type() == Object_t::ARR && other.type() == Object_t::NUM) {
			auto a = dynamic_cast<const Array*>(this);
			auto b = dynamic_cast<const Number*>(&other);
			return a->get(b->num());
		}
		else if (type() == Object_t::STR && other.type() == Object_t::NUM) {
			auto a = dynamic_cast<const String*>(this);
			auto b = dynamic_cast<const Number*>(&other);
			return a->get(b->num());
		}
		throw type_error("Cannot subscript", type(), other.type());
	}

	std::shared_ptr<Object>
	Object::dot_operator(const Object& right) const
	{
		if (right.type() == Object_t::ARR) {
			auto membercall = dynamic_cast<const Array*>(&right);
			if (has_method(membercall->begin()->to_str()))
				return invoke_method(membercall);
			else
				throw notamethod(membercall->begin()->to_str());
		}
		throw type_error("Invalid member/method call", type(), right.type());
	}

	// Conditional operators
	std::unique_ptr<Object>
	Object::operator==(const Object& other) const
	{
		if (type() == Object_t::NUM) {
			auto a = dynamic_cast<const Number*>(this);
			if (other.type() == Object_t::NUM) {
				auto b = dynamic_cast<const Number*>(&other);
				return std::make_unique<Bool>(a->num() == b->num());
			}
			else if (other.type() == Object_t::BOOL) {
				auto b = dynamic_cast<const Bool*>(&other);
				return std::make_unique<Bool>(a->is_truthy() == b->bol());
			}
		} else if (type() == Object_t::BOOL) {
			auto a = dynamic_cast<const Bool*>(this);
			if (other.type() == Object_t::BOOL) {
				auto b = dynamic_cast<const Bool*>(&other);
				return std::make_unique<Bool>(a->bol() == b->bol());
			}
			else if (other.type() == Object_t::NUM) {
				auto b = dynamic_cast<const Number*>(&other);
				return std::make_unique<Bool>(a->bol() == b->is_truthy());
			}
		} else if (type() == Object_t::STR && other.type() == Object_t::STR) {
			auto a = dynamic_cast<const String*>(this);
			auto b = dynamic_cast<const String*>(&other);
			return std::make_unique<Bool>(a->compare(*b));
		} else if (type() == Object_t::ARR && other.type() == Object_t::ARR) {
			auto a = dynamic_cast<const Array*>(this);
			auto b = dynamic_cast<const Array*>(&other);
			return std::make_unique<Bool>(a->compare(*b));
		}
		throw type_error("Cannot compare(==)", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator!=(const Object& other) const
	{
		if (type() == Object_t::NUM) {
			auto a = dynamic_cast<const Number*>(this);
			if (other.type() == Object_t::NUM) {
				auto b = dynamic_cast<const Number*>(&other);
				return std::make_unique<Bool>(a->num() != b->num());
			}
			else if (other.type() == Object_t::BOOL) {
				auto b = dynamic_cast<const Bool*>(&other);
				return std::make_unique<Bool>(a->is_truthy() != b->bol());
			}
		} else if (type() == Object_t::BOOL) {
			auto a = dynamic_cast<const Bool*>(this);
			if (other.type() == Object_t::BOOL) {
				auto b = dynamic_cast<const Bool*>(&other);
				return std::make_unique<Bool>(a->bol() != b->bol());
			}
			else if (other.type() == Object_t::NUM) {
				auto b = dynamic_cast<const Number*>(&other);
				return std::make_unique<Bool>(a->bol() != b->is_truthy());
			}
		} else if (type() == Object_t::STR && other.type() == Object_t::STR) {
			auto a = dynamic_cast<const String*>(this);
			auto b = dynamic_cast<const String*>(&other);
			return std::make_unique<Bool>(a->compare(*b));
		} else if (type() == Object_t::ARR && other.type() == Object_t::ARR) {
			auto a = dynamic_cast<const String*>(this);
			auto b = dynamic_cast<const String*>(&other);
			return std::make_unique<Bool>(a->compare(*b));
		}
		throw type_error("Cannot compare(!=)", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator&&(const Object& other) const
	{
		return std::make_unique<Bool>(this->is_truthy() && other.is_truthy());
	}

	std::unique_ptr<Object>
	Object::operator||(const Object& other) const
	{
		return std::make_unique<Bool>(this->is_truthy() || other.is_truthy());
	}

	std::unique_ptr<Object>
	Object::operator<(const Object& other) const
	{
		if (type() == Object_t::NUM && other.type() == Object_t::NUM) {
			auto a = dynamic_cast<const Number*>(this);
			auto b = dynamic_cast<const Number*>(&other);
			return std::make_unique<Bool>(a->num() < b->num());
		}
		throw type_error("Cannot compare(<)", type(), other.type());
	}

	// TODO: make this operator work with strings and arrays... maybe
	std::unique_ptr<Object>
	Object::operator<=(const Object& other) const
	{
		if (type() == Object_t::NUM && other.type() == Object_t::NUM) {
			auto a = dynamic_cast<const Number*>(this);
			auto b = dynamic_cast<const Number*>(&other);
			return std::make_unique<Bool>(a->num() <= b->num());
		}
		throw type_error("Cannot compare(<=)", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator>(const Object& other) const
	{
		if (type() == Object_t::NUM && other.type() == Object_t::NUM) {
			auto a = dynamic_cast<const Number*>(this);
			auto b = dynamic_cast<const Number*>(&other);
			return std::make_unique<Bool>(a->num() > b->num());
		}
		throw type_error("Cannot compare(>)", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator>=(const Object& other) const
	{
		if (type() == Object_t::NUM && other.type() == Object_t::NUM) {
			auto a = dynamic_cast<const Number*>(this);
			auto b = dynamic_cast<const Number*>(&other);
			return std::make_unique<Bool>(a->num() >= b->num());
		}
		throw type_error("Cannot compare(>=)", type(), other.type());
	}

	// Unary operators
	std::unique_ptr<Object>
	Object::operator-() const
	{
		if (type() == Object_t::NUM)
			return std::make_unique<Number>(-(dynamic_cast<const Number*>(this))->num());
		throw type_error("Cannot make negative", type());
	}

	std::unique_ptr<Object>
	Object::operator!() const
	{
		return std::make_unique<Bool>(!this->is_truthy());
	}

	// utility functions
	std::string
	Object::type_str() const
	{
		return Object::type_str(type());
	}

	std::string
	Object::type_str(Object_t type)
	{
		switch (type) {
			case Object_t::NUM: return "Number";
			case Object_t::STR: return "String";
			case Object_t::BOOL: return "Bool";
			case Object_t::ARR: return "Array";
			case Object_t::HASH: return "Hash";
			case Object_t::NIL: return "Nil";
			case Object_t::FILE: return "File";
		}
	}

	bool
	Object::has_method(const std::string& name) const
	{
		return methods_.find(name) != methods_.end();
	}

	std::shared_ptr<Object>
	Object::invoke_method(const Array* methodcall) const
	{
		auto name = methodcall->begin()->to_str();
		auto method = methods_.find(name)->second;
		std::vector<std::shared_ptr<Object>> args;
		args.push_back(std::const_pointer_cast<Object>(shared_from_this()));
		for (size_t i = 0; i < methodcall->size(); ++i)
			args.push_back(methodcall->get(i));
		args.erase(args.begin()+1); // delete methodname, inefficient for large numbers of args
		return method->invoke(args);
	}

	// binary error
	std::runtime_error
	Object::type_error(const std::string& msg, Object_t t1, Object_t t2) const
	{
		return std::runtime_error("error: " + msg + " '" + Object::type_str(t1) +
			"'' and '" + Object::type_str(t2) + "'.");
	}

	// unary error
	std::runtime_error
	Object::type_error(const std::string& msg, Object_t t1) const
	{
		return std::runtime_error("error: " + msg + " '" +
			Object::type_str(t1) + "'.");
	}

	std::runtime_error
	Object::notamethod(const std::string& name) const
	{
		return std::runtime_error("error: '" + name + "' is not a member of "
			+ this->to_str() + "'.");
	}

}
