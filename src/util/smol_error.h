#ifndef SMOL_ERROR_H
#define SMOL_ERROR_H
#include <exception>
#include <string>
#include "ansi.h"

class SmolError : public std::exception {
	public:
		~SmolError() = default;
		virtual void print() const = 0;
	
	protected:
		SmolError(const std::string& message)
			: message(message),
			  err(Color::FG_RED), secondary(Color::FG_BLUE), bold(Format::BOLD),
			  resetc(Color::FG_DEFAULT), resetf(Format::NORMAL) {}
		
		virtual std::string add_quotemarks(const std::string& s) const;
		virtual std::string get_file_line(int n) const;
		virtual void print_error_message() const;
		virtual void print_error_message_and_found(const std::string& found) const;
		virtual void print_error_line_number(int line) const;
		virtual void print_line_from_file(int line) const;
		virtual void print_line_from_file_bold(int line) const;
		virtual void print_abort_message() const;

		std::string message;
		ANSI::Modifier err;
    	ANSI::Modifier secondary;
    	ANSI::Modifier bold;
    	ANSI::Modifier resetc;
    	ANSI::Modifier resetf;
};

class SmolParserError : public SmolError {
	public:
		SmolParserError(const std::string &message, const std::string &expected,
						const std::string &found, int line)
			: SmolError{message},
			  expected(expected), found(found), line(line) {}
		
		void print() const override;
			  
	private:
		std::string expected;
		std::string found;
		int line;
};

class SmolLexerError : public SmolError {
	public:
		SmolLexerError(const std::string &message, const std::string &found, int line)
			: SmolError{message},
			  found(found), line(line) {}

		void print() const override;
		
	private:
		std::string found;
		int line;
};

class SmolCompilerError : public SmolError {
	public:
		SmolCompilerError(const std::string &message, const std::string &found)
			: SmolError{message}, found(found) {}

		void print() const override;
		
	private:
		std::string found;
};

#endif
