#ifndef ITEMINCREMENTS_HPP
#define ITEMINCREMENTS_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>

class ItemIncrements
{
public:
	ItemIncrements();
	~ItemIncrements();
	void add(int triggerminute, const std::string& what, const std::string& per, double howmuch);
	struct Increments
	{
		int trigger;
		std::string what;
		std::string per;
		double howmuch;
		friend bool operator== (const ItemIncrements::Increments &i1, const ItemIncrements::Increments &i2);
	};
	std::vector<Increments> allIncrements;
	void remove(int triggerminute, const std::string& what, const std::string& per, double howmuch);
	std::set<std::string> allItems;
private:
	std::fstream file;
};

#endif