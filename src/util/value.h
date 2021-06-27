/* value.h */
#ifndef VALUE_H
#define VALUE_H
#include <string>
#include <ostream>
#include <vector>

enum class Val_t {
    NUM, STR, BOOL, ARR, NIL,
};

class Val {

    public:
        Val(double val);
        Val(std::string val);
        Val(bool val);
        Val(std::vector<Val> arr);
        Val(); // NIL

        Val operator+ (Val const &other) const;
        Val operator- (Val const &other) const;
        Val operator/ (Val const &other) const;
        Val operator* (Val const &other) const;
        Val operator% (Val const &other) const;

        double get_num() const;
        std::string get_str() const;
        bool get_bool() const;
        std::vector<Val> get_arr() const;

        bool is_num() const;
        bool is_str() const;
        bool is_bool() const;
        bool is_arr() const;
        bool is_nil() const;
        bool same_type(Val const &val) const;
        bool is_truthy() const;

        std::string type_to_string() const;
        std::string to_str() const;

    private:
        Val_t type;
        double val_num;
        std::string val_str;
        bool val_bool;
        std::vector<Val> val_arr;
        int ndigits(double n) const;
};

#endif

