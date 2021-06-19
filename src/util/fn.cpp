#include "fn.h"
#include <stdexcept>
void
Fn::check_args(std::vector<Val> args, int count)
{
    if (args.size() != (unsigned)count)
        throw std::runtime_error(name() + ": arguments required (" +
            std::to_string(count) + ") but passed (" +
            std::to_string(args.size()) + ")");
}