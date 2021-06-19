#include "vartable.h"

auto VarTable::var_table =
    std::unordered_map<std::string, Val>();

void
VarTable::set_var(std::string name, Val val)
{
    var_table[name] = std::move(val);
}

bool
VarTable::exists(std::string name) const
{
    return var_table.find(name) != var_table.end();
}

Val
get_var(std::string name) const
{
    if (exists(name))
        return var_table[name];
    else { //throw new std::runtime_error();
        std::cout << "VarTable::get_var var does not exist.\n";
        return Val(); // nil
    }
}
