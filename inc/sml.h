#ifndef SML_H
#define SML_H

struct SML {
    bool had_error;

    SML();
    void run_prompt();    
    void run_file(std::string const& fname);
    void eval(std::string const& src);
    void error(int line, std::string const& message);
};

#endif

