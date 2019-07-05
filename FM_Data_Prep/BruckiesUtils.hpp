//Gotta make sure we don't include 'em all!
#ifndef _BRUECKIES_UTILS_HPP_

#include <string>
#include <vector>

//This function splits a string based on the limiter. Why this is not part of the
//STL is beyond me, but that's how life works, right?
//Courtesy of stackoverflow, https://stackoverflow.com/a/37454181 
//Optimizations (move, branch predictions, memory) and adjustments handcrafted. REEEEEEEEEE!
//anyway, string str is split by delim and returns a vector of strings, resulting from the split.
//we don't want this to be a const string since we might wanna fuck around with it later on. initialSize
//is supplied to allow a pre-allocation of vector spaces. one BIG alloc > 123 small allocs.

std::vector<std::string> split(const std::string& str, const std::string& delim, const unsigned int initialSize = 32)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}




#define _BRUCKIES_UTILS_HPP_
#endif