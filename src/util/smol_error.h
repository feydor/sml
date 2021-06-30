#ifndef SMOL_ERROR_H
#define SMOL_ERROR_H
#include <exception>
#include <string>
#include "ansi.h"

namespace Smol {
	class SyntaxError : public std::exception {
		public:
		SyntaxError(const std::string& msg, const std::string& expected,
			const std::string& found, int nline)
			: msg_(msg), expected_(expected), found_(found), nline_(nline) {};
		void print() const throw();

		private:
		std::string msg_;
		std::string expected_;
		std::string found_;
		int nline_;
		std::string to_quote(const std::string& s) const;
		std::string get_line() const;
	};

}
#endif
