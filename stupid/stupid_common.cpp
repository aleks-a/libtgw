#include "stupid_common.h"

#include <algorithm>
#include <cstdarg>

void trim(std::string &str)
{
	//if(str.length() == 0) { return; }
	std::string::iterator s;
	s = std::find_if(
		str.begin(),
		str.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace)));
	str.erase(str.begin(), s);
	
	//if(str.length() == 0) { return; }
	std::string::reverse_iterator e;
	e = std::find_if(
		str.rbegin(),
		str.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace)));
	str.erase(e.base(), str.end());
}

stupid_exception::stupid_exception():
	str("")
{
}

stupid_exception::stupid_exception(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsnprintf(str, sizeof(str), fmt, args);
	va_end(args);
}

const char * stupid_exception::what() const throw()
{
	return str;
}


