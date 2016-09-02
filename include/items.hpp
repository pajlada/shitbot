#ifndef ITEMS_H
#define ITEMS_H
#include <string>
#include "sqlite3.h"
#include <vector>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <map>
#include <algorithm>
#include <iostream>

class Items
{
public:
	Items();
	~Items();
	void stop();
	int createChannelTable(const std::string&);
	int addItemCategory(const std::string&);
	std::vector<std::string> getItems();
	std::vector<std::string> getColumnNames(const std::string& table);
	std::vector<std::string> getTableNames();
	std::vector<std::string> getChannels();
	int addIncrement(int trigger, const std::string& per, const std::string& what, const std::string& howmuch, bool percent);
	void getIncrements();
	void insertOrReplace(std::vector<std::string>);
	std::string getCount(const std::string&, const std::string&, const std::string&);
	struct Increments
	{
		int trigger;
		std::string per;
		std::string what;
		std::string howmuch;
		bool percent;
	};
	std::vector<Increments> _increments;
	int _maxTrigger;
private:
	sqlite3* _db;
};

#endif