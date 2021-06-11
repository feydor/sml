#include "value.h"
using namespace Val;

double Val::get_num()
{
    return val_num;
}

std::string Val::get_str()
{
    return val_str;
}

bool Val::get_bool()
{
    return val_bool;
}

bool Val::is_num()
{
    return this->type == Val::NUM;
}

bool Val::is_str()
{
    return this->type == Val::STR;
}

bool Val::is_bool()
{
    return this->type == Val::BOOL;
}

bool Val::is_nil()
{
    return this->type == Val::NIL;
}

std::string Val::type_to_string()
{
    switch(this->type) {
        case Val::NUM: return "NUM";
        case Val::STR: return "STR";
        case Val::BOOL: return "BOOL";
        case Val::NIL: return "NIL";
        case Val::ARRAY: return "ARRAY";
        case Val::IDENT: return "IDENT";
        case Val::OP: return "OP";
        case Val::GROUP: return "GROUP";
    }
}

Val::Val(double val)
    : type(Val::NUM), val_num(val) {};
Val::Val(std::string val)
    : type(Val::STR), val_num(val) {};
Val::Val(bool val)
    : type(Val::BOOL), val_bool(val) {};
Val::Val()
    : type(Val::NIL), val_num(0.0) {};
