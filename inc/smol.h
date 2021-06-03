#ifndef SMOL_H
#define SMOL_H

struct SMOL {
    static bool had_error;

    SMOL() {};
    static void run_prompt();
    static void run_file(std::string const& fname);
    static void eval(std::string const& src);
    static void error(int line, std::string const& message);
};

#endif

