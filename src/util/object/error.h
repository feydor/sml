#ifndef SMOL_OBJ_ERROR_H
#define SMOL OBJ_ERROR_H

namespace Obj {
class TypeError {
	public:
		TypeError()
		void print() const throw();
	private:
		std::string msg_;
		std::string op1_;
		std::string op2_;
		int nline_;
};

}

#endif
