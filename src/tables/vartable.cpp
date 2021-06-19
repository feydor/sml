#include "vartable.h"
#include <iostream>

auto VarTable::var_table =
    std::unordered_map<std::string, Val>();

void
VarTable::set_var(std::string name, Val val)
{
    VarTable::var_table[name] = std::move(val);
}

bool
VarTable::exists(std::string name)
{
    return VarTable::var_table.find(name) != VarTable::var_table.end();
}

std::unordered_map<std::string, Val>
VarTable::table()
{
    return VarTable::var_table;
}

Val
VarTable::get_var(std::string name)
{
    if (VarTable::exists(name))
        return VarTable::var_table[name];
    else { //throw new std::runtime_error();
        std::cout << "VarTable::get_var var does not exist.\n";
        return Val(); // nil
    }
}
