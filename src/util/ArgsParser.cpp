#include "ArgsParser.h"
#include <algorithm>

/**
 * @brief Construct a new Args Parser object
 * 
 * @param argc 
 * @param argv 
 */
ArgsParser::ArgsParser(int &argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
        this->tokens.push_back(std::string(argv[i]));
}

/**
 * @brief returns the argument followed by -option
 * 
 * @param option 
 * @return const std::string&, if option not found or not trailed by an argument, returns an empty string
 */
const std::string &
ArgsParser::get_cmd_opt(std::string_view option) const
{
    auto itr = std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end())
        return *itr;
    
    static const auto empty_string = std::string("");
    return empty_string;
}

/**
 * @brief 
 * 
 * @param option 
 * @return true 
 * @return false 
 */
bool
ArgsParser::cmd_opt_exists(std::string_view option) const
{
    return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
}