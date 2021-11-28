#pragma once
#include <string>
#include <vector>
#include <memory>

/**
 * @brief commandline parser
 * 
 */
class ArgsParser {
    public:
        ArgsParser(int &argc, char **argv);
        const std::string &get_cmd_opt(std::string_view option) const;
        bool cmd_opt_exists(std::string_view option) const;

    private:
        std::vector<std::string> tokens;
};
