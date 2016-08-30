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

class Items
{
public:
	Items();
	void startLoop();
	void stop();
private:
	std::thread _thread;
	std::condition_variable _cv;
	std::mutex _mutex;
	std::vector<int> _timeTriggers;
	std::map<std::string, int> _multiplyMap;
	int _maxTrigger;
	int _currentTrigger;
	std::atomic<bool> _quit;
	bool run();
	std::vector<int> getTimeTriggers();
	std::map<std::string, int> getMultipliers();
	void incrementLoop();
};