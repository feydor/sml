#ifndef SMOL_H
#define SMOL_H
#include <memory>
#include <string>

class Token;

struct SMOL {
    static bool had_error;
    static bool is_repl;
    static std::string fname;

    SMOL() {};
    static void run_prompt();
    static void run_file(std::string const &fname);
    static void eval(std::string const &src);
    static void error(int line, std::string const &message);
    static void error(Token const& tok, std::string const &message);
};

#endif

