#ifndef SMOL_FNTABLE_H
#define SMOL_FNTABLE_H
#include "ffi.h"
#include <string>
#include <unordered_map>

// a table that holds all of the current functions
// and their values, if defined
class FnTable {
    public:
    static void set_fn(std::shared_ptr<FFInterface> fn);
    static std::shared_ptr<FFInterface> get_fn(const std::string& name);
    static bool exists(const std::string& name);
    static std::string to_str();

    private:
    static std::unordered_map<std::string, std::shared_ptr<FFInterface>> fn_table;
};

#endif

