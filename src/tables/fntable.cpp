#include "fntable.h"

auto FnTable::fn_table =
    std::unordered_map<std::string, Fn*>();

void
FnTable::set_fn(Fn* fn)
{
    fn_table[fn.name()] = std::move(fn);
}

bool
FnTable::exists(std::string name) const
{
    return fn_table.find(name) != fn_table.end();
}

Fn*
get_fn(std::string name) const
{
    if (exists(name))
        return fn_table[name];
    else { //throw new std::runtime_error();
        std::cout << "FnTable::get_fn fn '" + name + "'does not exist.\n";
        return Val(); // nil
    }
}
