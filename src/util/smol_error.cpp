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

		std::cout << err << "error" << resetc << ": " << bold << msg_ << resetf << "\n" <<
			secondary << "  --> " << resetc << SMOL::fname + ":" + std::to_string(nline_) + "\n" <<
			secondary << "  |\n" << "  |    " << resetc << "- expected " + to_quote(expected_) + ", but found " +
			to_quote(found_) + ".\n" <<
			secondary << std::to_string(nline_) + " |    " << resetc << get_line() + "\n" <<
			secondary << "  |\n" << resetc;
		std::cout << err << "error" << resetc << ": " << bold << "aborting due to previous errors.\n" << resetf;
	}
	
	std::string
	SyntaxError::to_quote(const std::string& s) const
	{
		return "'" + s + "'";
	}

	std::string
	SyntaxError::get_line() const
	{
		std::string out;
		std::fstream file(SMOL::fname);
		file.seekg(std::ios::beg);
		for (int i = 0; i < nline_; ++i)
			std::getline(file, out);
		return out;
	}

}
