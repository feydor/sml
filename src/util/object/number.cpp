#include "object.h"
#include <sstream>
#include <cmath> // for std::trunc
#include <iomanip> // for std::setprecision


namespace Obj {
	Object_t
	Number::type() const
	{
		return Object_t::NUM;
	}

	// rounds to most significant ndigits
	std::string
	Number::to_str() const
	{
		std::stringstream sstream;
        sstream << std::fixed << std::setprecision(ndigits(num())) << num();
        return sstream.str();
	}

	bool
	Number::is_truthy() const
	{
		return num() == 0 ? false : true;
	}

	double
	Number::num() const
	{
		return val_;
	}

	// prefix increment
	std::shared_ptr<Object>
	Number::operator++()
	{
		++this->val_;
		return shared_from_this();
	}

	// prefix decrement
	std::shared_ptr<Object>
	Number::operator--()
	{
		--this->val_;
		return shared_from_this();
	}

	// gets the number of significant digits post-decimal
	int
	Number::ndigits(double n) const
	{
	    int digits = 0;
	    double frac = std::abs(n) - std::trunc(n);
	    constexpr double precision = 0.000001;
	    while (frac >= precision) {
	        frac *= 10.0;
	        digits++;
	        frac = frac - std::trunc(frac);
	    }
	    return digits;
	}

};
