#include "items.hpp"

Items::Items()
{
	_quit = false;
	_currentTrigger = 0;
	int rc;
	rc = sqlite3_open("items.db", &_db);
	if(rc)
	{
		throw std::runtime_error(sqlite3_errmsg(_db));
	}	
	char *error;
	const char* sql = "CREATE TABLE IF NOT EXISTS Increments(id INTEGER PRIMARY KEY ASC, trigger INTEGER, per TEXT, what TEXT, howmuch REAL, percent BOOL);";
	rc = sqlite3_exec(_db, sql, NULL, NULL, &error);
	if(rc)
	{
		fprintf(stderr, "Error executing Increments statement: %s\n", error);
		sqlite3_free(error);
		throw;
	}
	const char* sql2 = "CREATE TABLE IF NOT EXISTS Items(id INTEGER PRIMARY KEY ASC, name TEXT);";
	rc = sqlite3_exec(_db, sql2, NULL, NULL, &error);
	if(rc)
	{
		fprintf(stderr, "Error executing Items statement: %s\n", error);
		sqlite3_free(error);
		throw;
	}
}

Items::~Items()
{
	this->stop();
	sqlite3_close(_db);
}

void Items::startLoop()
{
	_thread = std::thread(&Items::incrementLoop, this);	
}

bool Items::getIncrements()
{
	_vecIncrements.clear();
	
	//get triggers from sql

	if(_vecIncrements.size() == 0)
	{
		return false;
	}
	else
	{
		_maxTrigger = (std::max_element(_vecIncrements.begin(), _vecIncrements.end(), [](const Increments& i1, const Increments& i2){return i1.trigger < i2.trigger;}))->trigger;
		return true;
	}
}

void Items::incrementLoop()
{
	while(!_quit)
	{
		std::unique_lock<std::mutex> lk(_mutex);
		if(_cv.wait_for(lk, std::chrono::minutes(1), [this](){return _quit.load();}))
		{
			// got notified to quit
			return;
		}
		else // 1 minute passed
		{
			if(!getIncrements())
			{
				continue;
			}
			++_currentTrigger;
			
			for(const Increments& i : _vecIncrements)
			{
				if(_currentTrigger % i.trigger) //increment correct intervals only
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