#ifndef SMOL_OBJECT_H
#define SMOL_OBJECT_H
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

namespace Obj {
enum class Object_t {
	NUM, STR, BOOL, NIL, ARR, HASH,
};

class Number;

class Object {
	public:
		virtual Object_t type() const = 0;
		virtual std::string to_str() const = 0;
		virtual bool is_truthy() const  = 0;
		virtual ~Object() = default;

		// Binary operators
		Object* operator+(const Object& other) const;
		Object* operator-(const Object& other) const;
		Object* operator*(const Object& other) const;
		Object* operator/(const Object& other) const;
		Object* operator%(const Object& other) const;

		// Conditional operators
		Object* operator&&(const Object& other) const;
		Object* operator||(const Object& other) const;
		Object* operator==(const Object& other) const;
		Object* operator!=(const Object& other) const;
		Object* operator<(const Object& other) const;
		Object* operator<=(const Object& other) const;
		Object* operator>(const Object& other) const;
		Object* operator>=(const Object& other) const;

		// Unary operators
		Object* operator-() const;
		Object* operator!() const;

		// utility
		std::string type_str() const;
		static std::string type_str(Object_t type);
		std::runtime_error type_error(const std::string& msg, Object_t, Object_t) const;
		std::runtime_error type_error(const std::string& msg, Object_t t1) const;
};

class Number : public Object {
	public:
		Object_t type() const override;
		std::string to_str() const override;
		bool is_truthy() const override;

		Number(double val) : val_(val) {};
		double num() const;
	private:
		double val_;
		int ndigits(double n) const;
};

class String : public Object {
	public:
		Object_t type() const override;
		std::string to_str() const override;
		bool is_truthy() const override;

		String(std::string val) : val_(val) {};
		std::string str() const;
		size_t size() const;
		bool compare(const String* other) const;
	private:
		std::string val_;
};

class Bool : public Object {
	public:
		Object_t type() const override;
		std::string to_str() const override;
		bool is_truthy() const override;

		Bool(bool val) : val_(val) {};
		bool bol() const;
	private:
		bool val_;
};

class Array : public Object {
	public:
		Object_t type() const override;
		std::string to_str() const override;
		bool is_truthy() const override;
		~Array() override;

		Array(std::vector<Object*> objects) : objects_(objects) {};
		Object* get(size_t i) const;
		void put(size_t i, Object* obj);
		void push_back(Object* obj);
		size_t size() const;
		bool compare(const Array* other) const;
	private:
		std::vector<Object*> objects_;
};

}

#endif
