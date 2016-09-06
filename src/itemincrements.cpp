#include "itemincrements.hpp"

void ItemIncrements::add(int triggerminute, const std::string& what, const std::string& per, double howmuch)
{
	if(allItems.count(what) == 1 && (allItems.count(per) == 1 || per == "default"))
	{
		allIncrements.push_back({triggerminute, what, per, howmuch});
	}
}

ItemIncrements::ItemIncrements()
{
	file.open("increments.txt", std::ios::in);
	Increments incr;
	while(file >> incr.trigger >> incr.what >> incr.per >> incr.howmuch)
	{
		allIncrements.push_back(incr);
	}
	file.close();
	itemsFile.open("allitems.txt", std::ios::in);
	std::string line;
	unsigned long long cost;
	while(itemsFile >> line >> cost)
	{
		allItems.insert({line, cost});
	}
	itemsFile.close();
	allItems.insert({"coin", 0});
	allItems.insert({"land", 1000});
}
	
ItemIncrements::~ItemIncrements()
{
	file.open("increments.txt", std::ios::out | std::ios::trunc);
	for(auto i : allIncrements)
	{
		file << i.trigger << " " << i.what << " " << i.per << " " << i.howmuch << std::endl;
	}
	file.close();
	itemsFile.open("allitems.txt", std::ios::out | std::ios::trunc);
	for(auto i : allItems)
	{
		itemsFile << i.first << " " << i.second << std::endl;
	}
	itemsFile.close();
}

void ItemIncrements::addNewItem(const std::string& what, unsigned long long cost)
{
	allItems.insert({what, cost});
}

bool operator== (const ItemIncrements::Increments &i1, const ItemIncrements::Increments &i2)
{
	return i1.trigger == i2.trigger && i1.what == i2.what && i1.per == i2.per && i1.howmuch == i2.howmuch;
}

void ItemIncrements::remove(int triggerminute, const std::string& what, const std::string& per, double howmuch)
{
	Increments incr{triggerminute, what, per, howmuch};
	auto it = std::find(allIncrements.begin(), allIncrements.end(), incr);
	if(it != allIncrements.end())
	{	
		allIncrements.erase(it);
	}
}