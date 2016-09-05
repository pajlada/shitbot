#ifndef CHANNELITEMS_HPP
#define CHANNELITEMS_HPP

#include <iostream>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <fstream>
#include <mutex>
#include <thread>

class ChannelItems
{
public:
	ChannelItems(const std::string& channel);
	void insert(const std::string& username);
	void insert(const std::string& username, const std::string& key, unsigned long long value);
	std::pair<bool, unsigned long long> get(const std::string& username, const std::string& key);
	size_t size();
	int readFile();
	int writeFile();
	void readAll();
	std::mutex* mtx;
private:
	std::unordered_map<std::string, std::map<std::string, unsigned long long>> usersMap;
	std::string m_channel;
};

#endif