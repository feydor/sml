/* ffi.cpp - foreign function interface (FFI) common methods */
#include "ffi.h"
#include <stdexcept>
void
FFInterface::check_nargs(std::vector<Val> args, int count)
{
    if (args.size() != (unsigned)count)
        throw std::runtime_error(name() + ": arguments required (" +
            std::to_string(count) + ") but passed (" +
            std::to_string(args.size()) + ")");
}
