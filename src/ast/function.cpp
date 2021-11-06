#include "prototype.h"

std::string
FunctionAST::get_name() const
{
    return prototype->get_name();
}

std::vector<std::string>
FunctionAST::get_args() const
{
    return prototype->get_args();
}