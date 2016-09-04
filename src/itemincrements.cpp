#include "itemincrements.hpp"

void ItemIncrements::add(int triggerminute, const std::string& what, const std::string& per, double howmuch)
{
	allIncrements.push_back({triggerminute, what, per, howmuch});
	allItems.insert(what);
}

ItemIncrements::ItemIncrements()
{
	file.open("increments.txt", std::ios::in);
	Increments incr;
	while(file >> incr.trigger >> incr.what >> incr.per >> incr.howmuch)
	{
		allIncrements.push_back(incr);
		allItems.insert(incr.what);
	}
	file.close();
}
	
ItemIncrements::~ItemIncrements()
{
	file.open("increments.txt", std::ios::out | std::ios::trunc);
	for(auto i : allIncrements)
	{
		file << i.trigger << " " << i.what << " " << i.per << " " << i.howmuch << std::endl;
	}
	file.close();
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