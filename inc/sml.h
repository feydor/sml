#ifndef SML_H
#define SML_H

struct SML {
    static bool had_error;

    SML();
    static void run_prompt();
    static void run_file(std::string const& fname);
    static void eval(std::string const& src);
    static void error(int line, std::string const& message);
};

#endif

