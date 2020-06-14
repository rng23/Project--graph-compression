#include <iostream>
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <utility>

typedef std::pair<std::string,std::string> pair;

int main()
{
	std::unordered_map<pair,int,boost::hash<pair>> unordered_map =
	{
		{{"C++", "C++11"}, 2011},
		{{"C++", "C++14"}, 2014},
		{{"C++", "C++17"}, 2017},
		{{"Java", "Java 7"}, 2011},
		{{"Java", "Java 8"}, 2014},
		{{"Java", "Java 9"}, 2017}
	};

	for (auto const &entry: unordered_map)
	{
		auto key_pair = entry.first;
		std::cout << "{" << key_pair.first << "," << key_pair.second << "}, "
				  << entry.second << '\n';
	}

	return 0;
}