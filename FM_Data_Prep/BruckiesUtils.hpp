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
	//Pre allocate memory to avoid several resizings
	//remember, if you set this to one you'll still get the same fuck ups. don't blame me if you do this
	tokens.reserve(initialSize);
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		pos = std::string::npos ? std::string::npos : str.length();
		tokens.push_back(str.substr(prev, pos - prev));
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	//Again I really don't understand why MSVC still doesn't implicitly produce move-semantics even with max. optis, but what the hell, 
	//a little explicit spice doesn't hurt now does it?
	return std::move(tokens);
}




#define _BRUCKIES_UTILS_HPP_
#endif