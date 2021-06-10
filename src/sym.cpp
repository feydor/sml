#include "sym.h"

void
SymTable::insert(Var *v)
{
    this->table.insert({v->sym, v});
}

bool
SymTable::in_table(std::string sym_name) const
{
    return this->table.find(sym_name) != table.end()
        ? true : false;
}
    
/* assumes sym_name is a valid key in table */
Sym *
SymTable::get(std::string sym_name) const
{
    return this->table.find(sym_name)->second;
}
