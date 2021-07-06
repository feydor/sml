#include "object.h"
#include "libfile.h"
#include <sstream>
#include <filesystem>

namespace Obj {
	File::File(const std::string& fname, const std::string& modes_str)
		: file_(new std::ifstream(fname)), fname_(fname)
	{
		parse_modes(modes_str);
		if (file_->is_open()) {
			is_open_ = true;
			size_ = std::filesystem::file_size(fname);
		}

		methods_.emplace("is_open", new Lib::File::is_open());
		methods_.emplace("fname", new Lib::File::fname());
		methods_.emplace("read", new Lib::File::read());
	};

	File::~File()
	{
		delete file_;
	}

	void
	File::parse_modes(const std::string& modes_str) {
		std::unordered_map<char, bool> mode_select = {
			{'i', false}, {'o', false}, {'b', false}, {'a', false}, {'t', false}
		};
		for (const auto& mode : modes_str) {
			switch (mode) {
				case 'i': if (!mode_set(mode_select, mode)) set_mode(std::ios::in); break;
				case 'o': if (!mode_set(mode_select, mode)) set_mode(std::ios::out); break;
				case 'b': if (!mode_set(mode_select, mode)) set_mode(std::ios::binary); break;
				case 'a': if (!mode_set(mode_select, mode)) set_mode(std::ios::app); break;
				case 't': if (!mode_set(mode_select, mode)) set_mode(std::ios::trunc); break;
			}
		}
	}

	Object_t
	File::type() const
	{
		return Object_t::FILE;
	}

	bool
	File::is_truthy() const
	{
		return is_open() ? true : false;
	}

	std::string
	File::to_str() const
	{
		return "<type File>";
	}

	std::string
	File::filename() const
	{
		return fname_;
	}

	std::uintmax_t
	File::size() const
	{
		return size_;
	}

	bool
	File::is_open() const
	{
		return is_open_;
	}

	std::string
	File::read() const
	{
		std::stringstream buf;
		buf << file_->rdbuf();
		return buf.str();
	}

	bool
	File::mode_set(std::unordered_map<char, bool>& mode_select, const char mode)
	{
		auto entry = mode_select.find(mode);
		if (entry == mode_select.end()) return false;
		return entry->second;
	}

	void
	File::set_mode(const std::ios_base::openmode& mode)
	{
		mode_ |= mode;
	}

}
