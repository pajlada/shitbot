#include "items.hpp"
#include <iostream>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <mutex>
#include "sqlite3.h"
#include <algorithm>

Items::Items()
{
	_timeTriggers = getTimeTriggers();
	_maxTrigger = std::max_element(_timeTriggers.begin(), _timeTriggers.end());
	_quit = false;
	_currentTrigger = 0;
}

void Items::startLoop()
{
	_th = std::thread(&Items::incrementLoop, this);	
}

std::vector<int> Items::getTimeTriggers()
{
	std::vector<int> vec;
	
	//get triggers from sql
	
	return vec;
}

std::map<std::string, int> Items::getMultipliers()
{
	std::map<std::string, int> map;
	
	//get multiply values from sql
	
	return map;
}

void Items::incrementLoop()
{
	while(run())
	{
		std::unique_lock<std::mutex> lk(_mutex);
		if(_cv.wait_for(lk, std::chrono::minutes(1), [this](){return _quit;}))
		{
			// got notified to quit
			return;
		}
		else // 1 minute passed
		{
			++_currentTrigger;
			
			for(const int& i : _timeTriggers)
			{
				if(_currentTrigger % i) //increment correct intervals only
				{
					
					//get twitch users
					//for each user increment the data in sql table
					//get old data, calculate new data, set new data in sql table
					
				}
			}
			
			if(_currentTrigger == _maxTrigger)
			{
				_currentTrigger = 0;
			}
		}
	}
}

bool Items::run()
{
	std::lock_guard<std::mutex> lk(_mutex);
	return !_quit;
}

void Items::stop()
{
	std::unique_lock<std::mutex> lk(_mutex);
	lk.lock();
	_quit = true;
	lk.unlock();
	_cv.notify_all();
	_thread.join();
	return;
}