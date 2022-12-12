#pragma once
#include <tuple>
#include <string>
#include <map>
#include <set>

class BlockReader
{
	std::set<std::string> types;

	class Block
	{
		std::map<std::string, Block> blocks;
		std::map<std::string, int> ints;
	};

	void Process()
	{

	}
};