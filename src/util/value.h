/* value.h */
#ifndef VALUE_H
#define VALUE_H
#include  <string>

namespace Val {
    enum Val_t {
        NUM, STR, BOOL, NIL, IDENT
    };

    class Val {
        Val_t type;
        double val_num;
        std::string val_str;
        bool val_bool;

        public:
        Val(double val);
        Val(std::string val);
        Val(bool val);
        Val(); // NIL, implemented as num_val = 0

        double get_num() const;
        std::string get_str() const;
        bool get_bool() const;

        bool is_num() const;
        bool is_str() const;
        bool is_bool() const;
        bool is_nil() const;
        bool same_type(Val const &val) const;
        
        std::string type_to_string() const;
        std::string to_string() const;
    };
}

#endif

