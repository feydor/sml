#ifndef SMOL_FN_H
#define SMOL_FN_H
#include "value.h"
#include <string>
#include <vector>

class Fn {
	public:
	virtual std::string name() = 0;
	virtual Val invoke(std::vector<Val> args) = 0;
	void check_args(std::vector<Val> args, int count);
	virtual ~Fn() {};
};

#endif