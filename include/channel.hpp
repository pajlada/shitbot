#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "asio.hpp"
#include <memory>
#include <map>
#include <string>
#include <chrono>
#include "eventqueue.hpp"
#include <iostream>

class Channel
{
public:
	Channel(const std::string &chn, std::shared_ptr<asio::ip::tcp::socket> sock, EventQueue<std::pair<std::unique_ptr<asio::streambuf>, std::string>> &evq) : sock{sock}, chn{chn}, eventQueue{evq}{}; //eShrug
	std::string chn;
	std::shared_ptr<asio::ip::tcp::socket> sock;
	unsigned int messageCount = 0;
	void run();
	bool quit();
	EventQueue<std::pair<std::unique_ptr<asio::streambuf>, std::string>> &eventQueue;
private:
	std::chrono::high_resolution_clock::time_point lastMessageTime;
	bool sendMsg(const std::string &msg);
};

#endif