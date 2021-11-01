#include "smol_error.h"
#include "smol.h"
#include <fstream>
#include <string>
#include <iostream>

namespace Smol {

	void
	SyntaxError::print() const throw()
	{
		/* tty modifiers */
		ANSI::Modifier err(Color::FG_RED);
    	ANSI::Modifier secondary(Color::FG_BLUE);
    	ANSI::Modifier bold(Format::BOLD);
    	ANSI::Modifier resetc(Color::FG_DEFAULT);
    	ANSI::Modifier resetf(Format::NORMAL);

		std::cout << err << "error" << resetc <<
			": " << bold << this->msg << resetf << "\n" <<
			secondary << "  --> " << resetc << SMOL::fname +
			":" + std::to_string(this->nline) + "\n" <<
			secondary << "  |\n" << "  |    " << resetc <<
			"- expected " + add_quotemarks(this->expected) + ", but found " +
			add_quotemarks(this->found) + ".\n" <<
			secondary << std::to_string(this->nline) + " |    "
			<< resetc << get_line(this->nline) + "\n" <<
			secondary << "  |\n" << resetc;
		std::cout << err << "error" << resetc << ": " << bold << "aborting due to previous errors.\n" << resetf;
	}
	
	// wraps the argument in strings and returns it
	std::string
	SyntaxError::add_quotemarks(const std::string& s) const
	{
		return "'" + s + "'";
	}

	// gets the nth line of the current file name
	std::string
	SyntaxError::get_line(int n) const
	{
		std::string out;
		std::fstream file(SMOL::fname);
		file.seekg(std::ios::beg);
		for (int i = 0; i < n; ++i)
			std::getline(file, out);
		return out;
	}

	void
	LexerError::print() const throw()
	{
		/* tty modifiers */
		ANSI::Modifier err(Color::FG_RED);
    	ANSI::Modifier secondary(Color::FG_BLUE);
    	ANSI::Modifier bold(Format::BOLD);
    	ANSI::Modifier resetc(Color::FG_DEFAULT);
    	ANSI::Modifier resetf(Format::NORMAL);

		std::cout << err << "error" << resetc <<
			": " << bold << this->msg << resetf << "\n" <<
			secondary << "  --> " << resetc << SMOL::fname +
			":" + std::to_string(this->nline) + "\n" <<
			secondary << "Found " + add_quotemarks(this->found) + ".\n" <<
			std::to_string(this->nline) + " |    "
			<< resetc << get_line(this->nline) + "\n" <<
			secondary << "  |\n" << resetc;
		// std::cout << err << "error" << resetc << ": " << bold << "aborting due to previous errors.\n" << resetf;
	}

}
