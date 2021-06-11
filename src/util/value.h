/* value.h */
#ifndef VALUE_H
#define VALUE_H

namespace Val {
    enum Val_t {
        NUM, STR, BOOL, NIL, ARRAY, GROUP, OP, IDENT
    };

    class Val {
        Val_t type;
        union {
            double val_num;
            std::string val_str;
            bool val_bool;
        };

        public:
        Val(double val);
        Val(std::string val);
        Val(bool val);
        Val(); // NIL, implemented as num_val = 0

        double get_num();
        std::string get_str();
        bool get_bool();

        bool is_num();
        bool is_str();
        bool is_bool();
        bool is_nil();
        
        std::string type_to_string();
    };
}

#endif

