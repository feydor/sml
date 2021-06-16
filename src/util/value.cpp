#include "value.h"

namespace Val {

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
        else if (other.is_str())
            return Val(this->get_str() + other.get_str());
    }

    // TODO: Show some sort of runtime/intpr error here
    return Val();
}

/* invalid data types returns NIL value */
Val
Val::operator- (Val const &other) const
{
   if (this->is_num())
       return Val(this->get_num() - other.get_num());

    // TODO: Show some sort of runtime/intpr error here
   return Val();
}

Val
Val::operator/ (Val const &other) const
{
   if (this->is_num())
       return Val(this->get_num() / other.get_num());

   return Val();
}

Val
Val::operator* (Val const &other) const
{
   if (this->is_num())
       return Val(this->get_num() * other.get_num());

   return Val();
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

bool
Val::is_truthy() const
{
    switch(this->type) {

        // 0 evaluates to false, all other numbers are true
        case NUM: return this->get_num() != 0;
        case STR: return false; // error
        case BOOL: return this->get_bool();
        case NIL: return false;
        case IDENT: return false; // error
    }
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

