#ifndef SMOL_ANSI_H
#define SMOL_ANSI_H
#include <ostream>
#include <unistd.h>
#include <stdio.h>

namespace Color {
	enum Code {
		FG_RED      = 31,
        FG_GREEN    = 32,
        FG_YELLOW   = 33,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
	};
}

namespace Format {
	enum Code {
		BOLD 	   = 1,
		DIM 	   = 2,
		UNDERLINED = 4,
		INVERT	   = 7,
		NORMAL     = 0,
	};
}

namespace ANSI {
	class Modifier {
		private:
		union {
			Color::Code col_code;
			Format::Code fmt_code;
		};

		enum mod_t { COLOR, FORMAT } type;

		public:
		explicit Modifier(Color::Code col) : col_code(col), type(COLOR) {};
		explicit Modifier(Format::Code fmt) : fmt_code(fmt), type(FORMAT) {};

		static bool istty() {
			return isatty(fileno(stdin));
		}

		friend std::ostream&
		operator<< (std::ostream& os, Modifier const &mod) {
			return mod.type == COLOR
					? os << "\033[" << mod.col_code << "m"
				: os << "\033[" << mod.fmt_code << "m";
		}
	};
}
#endif
