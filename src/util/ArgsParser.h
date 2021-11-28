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
        const std::string &get_cmd_opt(const std::string &option) const;
        bool cmd_opt_exists(const std::string &option) const;

    private:
        std::vector<std::string> tokens;
};
