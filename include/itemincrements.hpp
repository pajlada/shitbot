#ifndef ITEMINCREMENTS_HPP
#define ITEMINCREMENTS_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>

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
	std::map<std::string, unsigned long long> allItems;
	void addNewItem(const std::string& what, unsigned long long cost);
private:
	std::fstream file;
	std::fstream itemsFile;
};

#endif