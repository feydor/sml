#ifndef SMOL_OBJECT_H
#define SMOL_OBJECT_H
//#include "ffi.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <unordered_map>

class FFInterface;

namespace Obj {
enum class Object_t {
	NUM, STR, BOOL, NIL, ARR, HASH, FILE,
};
class Number;
class Array;

class Object : public std::enable_shared_from_this<Object> {
	public:
		virtual Object_t type() const = 0;
		virtual std::string to_str() const = 0; // used to compare Arrays' contents
		virtual bool is_truthy() const = 0;
		virtual ~Object() = default;
		bool has_method(const std::string& name) const;
		std::shared_ptr<Object> invoke_method(const Array* call) const;

		// Binary operators
		std::unique_ptr<Object> operator+(const Object& other) const;
		std::unique_ptr<Object> operator-(const Object& other) const;
		std::unique_ptr<Object> operator*(const Object& other) const;
		std::unique_ptr<Object> operator/(const Object& other) const;
		std::unique_ptr<Object> operator%(const Object& other) const;
		std::shared_ptr<Object> operator[](const Object& other) const;
		std::shared_ptr<Object> dot_operator(const Object& right) const;

		// Conditional operators
		std::unique_ptr<Object> operator&&(const Object& other) const;
		std::unique_ptr<Object> operator||(const Object& other) const;
		std::unique_ptr<Object> operator==(const Object& other) const;
		std::unique_ptr<Object> operator!=(const Object& other) const;
		std::unique_ptr<Object> operator<(const Object& other) const;
		std::unique_ptr<Object> operator<=(const Object& other) const;
		std::unique_ptr<Object> operator>(const Object& other) const;
		std::unique_ptr<Object> operator>=(const Object& other) const;

		// Unary operators
		std::unique_ptr<Object> operator-() const;
		std::unique_ptr<Object> operator!() const;

		// utility
		std::string type_str() const;
		static std::string type_str(Object_t type);
		std::runtime_error type_error(const std::string& msg, Object_t, Object_t) const;
		std::runtime_error type_error(const std::string& msg, Object_t) const;
		std::runtime_error notamethod(const std::string& name) const;
		std::unordered_map<std::string, FFInterface*> methods() const;

	protected:
		std::unordered_map<std::string, FFInterface*> methods_;
};

class Number : public Object {
	public:
		Object_t type() const override;
		std::string to_str() const override;
		bool is_truthy() const override;

		explicit Number(double val) : val_(val) {};
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

		explicit String(const std::string& val);
		std::string str() const;
		size_t size() const;
		std::shared_ptr<Object> get(size_t i) const;
		bool compare(const String& other) const;
	private:
		std::string val_;
};

class Bool : public Object {
	public:
		Object_t type() const override;
		std::string to_str() const override;
		bool is_truthy() const override;

		explicit Bool(bool val) : val_(val) {};
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

		explicit Array(const std::vector<std::shared_ptr<Object>>& objects);
		std::shared_ptr<Object> get(size_t i) const;
		void put(size_t i, std::shared_ptr<Object> obj);
		void push_back(std::shared_ptr<Object> obj);
		size_t size() const;
		bool compare(const Array& other) const;
		std::shared_ptr<Object> begin() const;
	private:
		std::vector<std::shared_ptr<Object>> objects_;
};

class File : public Object {
	public:
		Object_t type() const override;
		std::string to_str() const override;
		bool is_truthy() const override;

		explicit File(const std::string& fname, const std::string& modes_str);
		~File() override;
		std::uintmax_t size() const;
		std::string read() const;
		//std::vector<std::string> get_lines() const;
		std::string filename() const;
		bool is_open() const;
	private:
		std::ifstream* file_;
		std::ios_base::openmode mode_;
		std::string fname_;
		std::uintmax_t size_;
		bool is_open_ = false;

		void parse_modes(const std::string& modes_str);
		bool mode_set(std::unordered_map<char, bool>& mode_select, const char mode);
		void set_mode(const std::ios_base::openmode& mode);
};

}

#endif
