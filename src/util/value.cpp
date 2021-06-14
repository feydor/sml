#include "value.h"

namespace Val {

Val
Val::operator+ (Val const &other) const
{
    Val temp;
    if (this->is_num()) {
        if (other.is_num()) {
            temp.val_num = this->get_num() + other.get_num();
            temp.type = NUM;
        } else if (other.is_str()) {
            temp.val_str = std::to_string(this->get_num()) + other.get_str();
            temp.type = STR;
        }
    } else if (this->is_str()) {
        if (other.is_num()) {
            temp.val_str = this->get_str() + std::to_string(other.get_num());
            temp.type = STR;
        } else if (other.is_str()) {
            temp.val_str = this->get_str() + other.get_str();
            temp.type = STR;
        }
    }
    return temp;
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

bool
Val::is_num() const
{
    return this->type == NUM;
}

bool
Val::is_str() const
{
    return this->type == STR;
}

bool
Val::is_bool() const
{
    return this->type == BOOL;
}

bool
Val::is_nil() const
{
    return this->type == NIL;
}
        
bool
Val::same_type(Val const &val) const
{
    return this->type == val.type;
}

std::string
Val::type_to_string() const
{
    switch(this->type) {
        case NUM: return "NUM";
        case STR: return "STR";
        case BOOL: return "BOOL";
        case NIL: return "NIL";
        case IDENT: return "IDENT";
    }
}

std::string
Val::to_string() const
{
    switch (this->type) {
        case NUM: return std::to_string(get_num());
        case STR: return get_str();
        case BOOL: return get_bool() ? "true" : "false";
        case NIL: return "nil";
        case IDENT: return "ERROR: Ident on stack after eval.";
    }
}
Val::Val(double val)
    : type(NUM), val_num(val) {};
Val::Val(std::string val)
    : type(STR), val_str(val) {};
Val::Val(bool val)
    : type(BOOL), val_bool(val) {};
Val::Val()
    : type(NIL), val_num(0.0) {};
}

