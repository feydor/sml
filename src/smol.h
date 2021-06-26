#ifndef SMOL_H
#define SMOL_H
#include <memory>
#include <string>

class Tok;

struct SMOL {
    static bool had_error;
    static bool is_repl;
    static bool benchmark;
    static std::string fname;

    SMOL() {};
    static void run_prompt();
    static void run_file(std::string const &fname);
    static void eval(std::string const &src);
    static void error(int line, std::string const &message);
    static void print_usage();
    static void print_version();
    static void error(Tok const& tok, std::string const &message);
};

#endif

