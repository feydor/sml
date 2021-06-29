#ifndef SMOL_OBJECT_H
#define SMOL_OBJECT_H
#include <string>
#include <vector>
#include <memory>

namespace Smol {
enum class Object_t {
	NUM, STR, BOOL, NIL, ARR, HASH,
};

class Object {
	public:
		virtual Object_t type() const = 0;
		virtual std::string to_str() const = 0;
		virtual bool is_truthy() const  = 0;
		virtual std::unique_ptr<Object> operator+(const Object& other) const = 0;
		virtual ~Object() = 0;
};

class Number : public Object {
	public:
		Object_t type() const override;
		std::string to_str() const override;
		bool is_truthy() const override;
		std::unique_ptr<Object> operator+(const Object& other) const override;

		Number(double val) : val_(val);
		double get() const;
	private:
		double val_;
};

class String : public Object {
	public:
		Object_t type() const override;
		std::string to_str() const override;
		bool is_truthy() const override;
		std::unique_ptr<Object> operator+(const Object& other) const override;

		String(std::string val) : val_(val);
		std::string get() const;
	private:
		std::string val_;
};

class Bool : public Object {
	public:
		Object_t type() const override;
		std::string to_str() const override;
		bool is_truthy() const override;
		std::unique_ptr<Object> operator+(const Object& other) const override;

		Bool(bool val) : val_(val);
		bool get() const;
	private:
		bool val_;
};

class Array : public Object {
	public:
		Object_t type() const override;
		std::string to_str() const override;
		bool is_truthy() const override;
		std::unique_ptr<Object> operator+(const Object& other) const override;
		~Array() override;

		Object* get(size_t i) const;
		void put(size_t i, Object* obj);
		void push_back(Object* obj);
	private:
		std::vector<Object*> objects_;
};

}

#endif
