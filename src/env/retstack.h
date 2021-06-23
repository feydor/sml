#ifndef SMOL_RETSTACK_H
#define SMOL_RETSTACK_H
#include "value.h"
#include <vector>
#include <string>

class RetStack {
    public:
    static void push_back(Val v);
    static Val pop();
    static std::string to_str();
    static bool is_empty();

    private:
    static std::vector<Val> rstack;
};

#endif