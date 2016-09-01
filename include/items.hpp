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
	void startLoop();
	void stop();
	int createChannelTable(const std::string&);
	int addItemCategory(const std::string&);
	std::vector<std::string> getItems();
	std::vector<std::string> getColumnNames(const std::string& table);
	std::vector<std::string> getTableNames();
private:
	struct Increments
	{
		int trigger;
		std::string per;
		std::string what;
		double howmuch;
		bool percent;
	};
	sqlite3* _db;
	std::thread _thread;
	std::condition_variable _cv;
	std::mutex _mutex;
	int _maxTrigger;
	int _currentTrigger;
	std::atomic<bool> _quit;
	bool getIncrements();
	std::vector<Increments> _vecIncrements;
	void incrementLoop();
};

#endif