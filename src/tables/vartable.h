#ifndef SMOL_VARTABLE_H
#define SMOL_VARTABLE_H
#include "value.h"
#include <string>

// a table that holds all of the current vars
// and their values, if defined
class VarTable {
    public:
    static void set_var(std::string name, Val val);
    static Val get_var(std::string name) const;
    static bool exists(std::string name) const;
    
    private:
    static std::unordered_map<std::string, Val> var_table;
};

#endif

