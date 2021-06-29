#include "object.h"

class Var {
	public:
		Var(const std::string& name, Obj::Object* val)
			: name_(name), val_(val) {};
		~Var() {
			delete object_;
		};
	private:
		std::string name_;
		Obj::Object* object_;
};
