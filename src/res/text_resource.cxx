#include "text_resource.hxx"

#include <string>
#include <fstream>
#include <cassert>
#include <iostream>

namespace res
{
	void TextResource::load(const std::string path)
	{
		std::ifstream fs(path, std::ios::ate);
		const long size{fs.tellg()};
		fs.seekg(0);
		text = new char[size + 1];
		for (long int c{0}; c < size; ++c) {
			text[c] = fs.get();
		}
		fs.close();
		text[size] = '\0';
	}

	void TextResource::unload()
	{
		delete text;
	}

	const char *TextResource::get_text() const
	{
		return text;
	}
}
