#include "smol_error.h"
#include "smol.h"
#include <fstream>
#include <string>

namespace Smol {

	const std::string
	SyntaxError::to_str() const throw()
	{
		/* tty modifiers */
		ANSI::Modifier err(Color::FG_RED);
    	ANSI::Modifier secondary(Color::FG_BLUE);
    	ANSI::Modifier bold(Format::BOLD);
    	ANSI::Modifier def(Color::FG_DEFAULT);

		std::string err_str("");
		err_str += "error: " + msg_ + "\n" + "  --> " + SMOL::fname + ":" + std::to_string(nline_) + "\n" +
			"  |\n" + "  |    " + "- expected " + to_quote(expected_) + ", but found " +
			to_quote(found_) + ".\n" + std::to_string(nline_) + " |    " + get_line() + "\n" +
			"  |\n";
		err_str += "error: aborting due to previous errors.";
		return err_str;
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
