#include "fn.h"
#include "fntable.h"
#include <iostream>

auto FnTable::fn_table =
    std::unordered_map<std::string, Fn*>();

void
FnTable::set_fn(Fn* fn)
{
    fn_table[fn->name()] = std::move(fn);
}

bool
FnTable::exists(std::string name)
{
    return fn_table.find(name) != fn_table.end();
}

Fn*
FnTable::get_fn(std::string name)
{
    if (exists(name))
        return fn_table[name];
    else { //throw new std::runtime_error();
        std::cout << "FnTable::get_fn fn '" + name + "'does not exist.\n";
        return nullptr;
    }
}
