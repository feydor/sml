#ifndef SMOL_ERROR_H
#define SMOL_ERROR_H
#include <exception>
#include <string>
#include "ansi.h"

namespace Smol {

	class SyntaxError : public std::exception {
		
		public:
		virtual ~SyntaxError() = default;
		virtual void print() const throw();

		protected:
		SyntaxError(const std::string &msg, const std::string &expected,
			const std::string &found, int nline)
			: msg(msg), expected(expected), found(found), nline(nline) {};

		std::string msg;
		std::string expected;
		std::string found;
		int nline;
		virtual std::string add_quotemarks(const std::string& s) const;
		virtual std::string get_line(int n) const;
	};

	// ParserError - For syntax errors found during parsing
	class ParserError : public SyntaxError {
		public:
		ParserError(const std::string &msg, const std::string &expected,
			const std::string &found, int nline)
			: SyntaxError{msg, expected, found, nline} {};
	};

	class LexerError : public SyntaxError {
		public:
		LexerError(const std::string &msg, const std::string &found, int nline)
			: SyntaxError{msg, "", found, nline} {};
		
		void print() const throw() override;
	};

}
#endif
