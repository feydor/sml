#include "object.h"
#include "ffi.h"
#include <cmath>  // for std::fmod
#include <iostream>

namespace Obj {
	
	// Binary operators
	std::unique_ptr<Object>
	Object::operator+(const Object& other) const
	{
		const Object* a = this;
		const Object* b = &other;
		if (type() == Object_t::NUM) {
			if (other.type() == Object_t::NUM)
				return std::make_unique<Number>(((Number*)a)->num() + ((Number*)b)->num());
			else if (other.type() == Object_t::STR)
				return std::make_unique<String>(((Number*)a)->to_str() + ((String*)b)->str());
		}
		else if (type() == Object_t::STR) {
			if (other.type() == Object_t::STR)
				return std::make_unique<String>(((String*)a)->str() + ((String*)b)->str());
			else if (other.type() == Object_t::NUM)
				return std::make_unique<String>(((String*)a)->str() + ((Number*)b)->to_str());
		}
		else if (type() == Object_t::BOOL) {
			if (other.type() == Object_t::BOOL)
				return std::make_unique<Bool>(((Bool*)a)->bol() && ((Bool*)b)->bol());
		}
		throw type_error("Cannot add", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator-(const Object& other) const
	{
		const Object* a = this;
		const Object* b = &other;
		if (type() == Object_t::NUM && other.type() == Object_t::NUM)
			return std::make_unique<Number>(((Number*)a)->num() - ((Number*)b)->num());
		throw type_error("Cannot subtract", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator*(const Object& other) const
	{
		const Object* a = this;
		const Object* b = &other;
		if (type() == Object_t::NUM && other.type() == Object_t::NUM)
			return std::make_unique<Number>(((Number*)a)->num() * ((Number*)b)->num());
		throw type_error("Cannot multiply", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator/(const Object& other) const
	{
		const Object* a = this;
		const Object* b = &other;
		if (type() == Object_t::NUM && other.type() == Object_t::NUM)
			return std::make_unique<Number>(((Number*)a)->num() / ((Number*)b)->num());
		throw type_error("Cannot divide", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator%(const Object& other) const
	{
		const Object* a = this;
		const Object* b = &other;
		if (type() == Object_t::NUM && other.type() == Object_t::NUM)
			return std::make_unique<Number>(std::fmod(((Number*)a)->num(), ((Number*)b)->num()));
		throw type_error("Cannot mod", type(), other.type());
	}

	std::shared_ptr<Object>
	Object::operator[](const Object& other) const
	{
		const Object* a = this;
		const Object* b = &other;
		if (type() == Object_t::ARR && other.type() == Object_t::NUM)
			return ((Array*)a)->get(((Number*)b)->num());
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
		const Object* a = this;
		const Object* b = &other;
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
			if (other.type() == Object_t::BOOL)
				return std::make_unique<Bool>(((Bool*)a)->bol() == ((Bool*)b)->bol());
			else if (other.type() == Object_t::NUM)
				return std::make_unique<Bool>(((Bool*)a)->bol() == ((Number*)b)->is_truthy());
		} else if (type() == Object_t::STR) {
			if (other.type() == Object_t::STR)
				return std::make_unique<Bool>(((String*)a)->compare(*(String*)b));
		} else if (type() == Object_t::ARR) {
			if (other.type() == Object_t::ARR)
				return std::make_unique<Bool>(((Array*)a)->compare(*(Array*)b));
		}
		throw type_error("Cannot compare(==)", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator!=(const Object& other) const
	{
		const Object* a = this;
		const Object* b = &other;
		if (type() == Object_t::NUM) {
			if (other.type() == Object_t::NUM)
				return std::make_unique<Bool>(((Number*)a)->num() != ((Number*)b)->num());
			else if (other.type() == Object_t::BOOL)
				return std::make_unique<Bool>(((Number*)a)->is_truthy() != ((Bool*)b)->bol());
		} else if (type() == Object_t::BOOL) {
			if (other.type() == Object_t::BOOL)
				return std::make_unique<Bool>(((Bool*)a)->bol() != ((Bool*)b)->bol());
			else if (other.type() == Object_t::NUM)
				return std::make_unique<Bool>(((Bool*)a)->bol() != ((Number*)b)->is_truthy());
		} else if (type() == Object_t::STR) {
			if (other.type() == Object_t::STR)
				return std::make_unique<Bool>(((String*)a)->compare(*(String*)b));
		} else if (type() == Object_t::ARR) {
			if (other.type() == Object_t::ARR)
				return std::make_unique<Bool>(((Array*)a)->compare(*(Array*)b));
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
		const Object* a = this;
		const Object* b = &other;
		if (type() == Object_t::NUM && other.type() == Object_t::NUM)
			return std::make_unique<Bool>(((Number*)a)->num() < ((Number*)b)->num());
		throw type_error("Cannot compare(<)", type(), other.type());
	}

	// TODO: make this operator work with strings and arrays... maybe
	std::unique_ptr<Object>
	Object::operator<=(const Object& other) const
	{
		const Object* a = this;
		const Object* b = &other;
		if (type() == Object_t::NUM && other.type() == Object_t::NUM)
			return std::make_unique<Bool>(((Number*)a)->num() <= ((Number*)b)->num());
		throw type_error("Cannot compare(<=)", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator>(const Object& other) const
	{
		const Object* a = this;
		const Object* b = &other;
		if (type() == Object_t::NUM && other.type() == Object_t::NUM)
			return std::make_unique<Bool>(((Number*)a)->num() > ((Number*)b)->num());
		throw type_error("Cannot compare(>)", type(), other.type());
	}

	std::unique_ptr<Object>
	Object::operator>=(const Object& other) const
	{
		const Object* a = this;
		const Object* b = &other;
		if (type() == Object_t::NUM && other.type() == Object_t::NUM)
			return std::make_unique<Bool>(((Number*)a)->num() >= ((Number*)b)->num());
		throw type_error("Cannot compare(>=)", type(), other.type());
	}

	// Unary operators
	std::unique_ptr<Object>
	Object::operator-() const
	{
		if (type() == Object_t::NUM)
			return std::make_unique<Number>(-((Number*)this)->num());
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
		/*
		for (const auto& obj : methodcall)
			args.push_back(obj->to_str());
			*/
		args.erase(args.begin()); // NOTE: inefficient for large numbers of args
		for (const auto& arg : args)
			std::cout << arg->to_str() << std::endl;
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
