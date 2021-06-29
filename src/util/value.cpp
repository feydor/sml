#include "value.h"
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iomanip>

/* invalid data types returns NIL value */
Val
Val::operator+ (Val const &other) const
{
    if (this->is_num()) {
        if (other.is_num())
            return Val(this->get_num() + other.get_num());
        else if (other.is_str())
            return Val(std::to_string(this->get_num()) + other.get_str());
    } else if (this->is_str()) {
        if (other.is_num())
            return Val(this->get_str() + std::to_string(other.get_num()));
        if (other.is_str())
            return Val(this->get_str() + other.get_str());
    }
    throw std::runtime_error("Invalid operand types in binary operator '+'.");
}

/* invalid data types returns NIL value */
Val
Val::operator- (Val const &other) const
{
   if (this->is_num())
       return Val(this->get_num() - other.get_num());
  throw std::runtime_error("Invalid operand types in binary operator '-'.");
}

Val
Val::operator/ (Val const &other) const
{
   if (this->is_num())
       return Val(this->get_num() / other.get_num());
   throw std::runtime_error("Invalid operand types in binary operator '/'.");
}

Val
Val::operator* (Val const &other) const
{
   if (this->is_num())
       return Val(this->get_num() * other.get_num());
   throw std::runtime_error("Invalid operand types in binary operator '*'.");
}

Val
Val::operator% (Val const &other) const
{
    if (this->is_num() && other.is_num())
        return Val(std::fmod(this->get_num(), other.get_num()));
    throw std::runtime_error("Invalid operand types in binary operator '%'.");
}

double
Val::get_num() const
{
    return val_num;
}

std::string
Val::get_str() const
{
    return val_str;
}

bool
Val::get_bool() const
{
    return val_bool;
}

std::vector<Val>
Val::get_arr() const
{
    return val_arr;
}

bool
Val::is_num() const
{
    return this->type == Val_t::NUM;
}

bool
Val::is_str() const
{
    return this->type == Val_t::STR;
}

bool
Val::is_bool() const
{
    return this->type == Val_t::BOOL;
}

bool
Val::is_arr() const
{
    return this->type == Val_t::ARR;
}

bool
Val::is_nil() const
{
    return this->type == Val_t::NIL;
}

bool
Val::same_type(Val const &val) const
{
    return this->type == val.type;
}

bool
Val::is_truthy() const
{
    switch(this->type) {
        // 0 evaluates to false, all other numbers are true
        case Val_t::NUM: return this->get_num() != 0;
        case Val_t::STR: return false; // error
        case Val_t::BOOL: return this->get_bool();
        case Val_t::ARR: return false;
        case Val_t::NIL: return false;
    }
}

std::string
Val::type_to_string() const
{
    switch(this->type) {
        case Val_t::NUM: return "NUM";
        case Val_t::STR: return "STR";
        case Val_t::BOOL: return "BOOL";
        case Val_t::ARR: return "ARR";
        case Val_t::NIL: return "NIL";
    }
}

std::string
Val::to_str() const
{
    switch (this->type) {
        case Val_t::NUM:
        {
            std::stringstream sstream;
            sstream << std::fixed << std::setprecision(ndigits(get_num())) << get_num();
            return sstream.str();
        }
        case Val_t::STR: return get_str();
        case Val_t::BOOL: return get_bool() ? "true" : "false";
        case Val_t::ARR:
        {
            std::string s;
            for (Val val : get_arr()) {
                s += val.to_str();
                s += " ";
            }
            return s;
        }
        case Val_t::NIL: return "nil";
    }
}

// gets the number of significant digits post-decimal
int
Val::ndigits(double n) const
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

Val::Val(double val)
    : type(Val_t::NUM), val_num(val) {};
Val::Val(std::string val)
    : type(Val_t::STR), val_str(val) {};
Val::Val(bool val)
    : type(Val_t::BOOL), val_bool(val) {};
Val::Val(std::vector<Val> arr)
    : type(Val_t::ARR), val_arr(arr) {};
Val::Val()
    : type(Val_t::NIL), val_num(0.0) {};

