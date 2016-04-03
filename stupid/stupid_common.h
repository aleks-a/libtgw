#ifndef STUPID__STUPID_COMMON__H
#define STUPID__STUPID_COMMON__H

#include <fstream>
#include <iostream>
#include <string>

#include <cstdio>
#include <cstdlib>

#define s_info(...)		printf(__VA_ARGS__)
#define s_err(...)		fprintf(stderr, __VA_ARGS__)

void trim(std::string &str);

inline const char * bool_str(bool b)
{
	return b ? "true" : "false";
}

class stupid_exception: virtual public std::exception
{
public:
	stupid_exception();
	stupid_exception(const char *msg, ...);
	virtual const char * what() const throw();
	
private:
	char str[4096];
};


#endif
