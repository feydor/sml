#include "fn.h"

void
Fn::check_args(std::vector<Val> args, int count)
{
    if (args.size() != count)
        throw std::runtime_error(name() + ": arguments required ("
            +count + ") but passed (" + args.size() + ")");
}