#include "fntable.h"
#include <iostream>

auto FnTable::fn_table =
    std::unordered_map<std::string, FFInterface*>();

void
FnTable::set_fn(FFInterface* fn)
{
    fn_table[fn->name()] = fn;
}

bool
FnTable::exists(std::string name)
{
    return fn_table.find(name) != fn_table.end();
}

FFInterface*
FnTable::get_fn(std::string name)
{
    if (exists(name))
        return fn_table[name];
    else { //throw new std::runtime_error();
        std::cout << "FnTable::get_fn fn '" + name + "'does not exist.\n";
        return nullptr;
    }
}

std::string
FnTable::to_str()
{
    std::string out("");
    out += "--------FN_TABLE-------\n";
    out += "size: " + std::to_string(fn_table.size()) + "\n";
    for (auto fn_pair : fn_table) {
        out += fn_pair.first + "\n";
    }
    return out;
}

FnTable::~FnTable()
{
    for (auto fn : fn_table)
        delete fn.second;
}
