#ifndef SMOL_FNTABLE_H
#define SMOL_FNTABLE_H
#include "value.h"
#include "ffi.h"
#include <string>
#include <unordered_map>

// a table that holds all of the current functions
// and their values, if defined
class FnTable {
    public:
    static void set_fn(FFInterface* fn);
    static FFInterface* get_fn(std::string name);
    static bool exists(std::string name);
    static std::string to_str();
    ~FnTable();
    
    private:
    static std::unordered_map<std::string, FFInterface*> fn_table;
};

#endif

