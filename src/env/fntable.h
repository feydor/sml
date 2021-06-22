#ifndef SMOL_FNTABLE_H
#define SMOL_FNTABLE_H
#include "value.h"
#include "fn.h"
#include <string>
#include <unordered_map>

// a table that holds all of the current functions
// and their values, if defined
class FnTable {
    public:
    static void set_fn(Fn* fn);
    static Fn* get_fn(std::string name);
    static bool exists(std::string name);
    static std::string to_str();
    
    private:
    static std::unordered_map<std::string, Fn*> fn_table;
};

#endif

