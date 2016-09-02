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
	int addIncrement(int trigger, const std::string& per, const std::string& what, double howmuch);
	void getIncrements();
	void insertOrReplace(const std::string& channel, const std::string& username, const std::string& what, long long howmany);
	long long getCount(const std::string&, const std::string&, const std::string&);
	struct Increments
	{
		int trigger;
		std::string per;
		std::string what;
		double howmuch;
	};
	std::vector<Increments> _increments;
	int deleteIncrement(int);
	void begin();
	void end();
private:
	sqlite3* _db;
};

#endif