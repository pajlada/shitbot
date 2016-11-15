#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "asio.hpp"
#include <memory>
#include <map>
#include <string>
#include <chrono>
#include "eventqueue.hpp"
#include <iostream>
#include <atomic>

class Channel
{
public:
	Channel(const std::string &chn, std::shared_ptr<asio::ip::tcp::socket> sock, EventQueue<std::pair<std::unique_ptr<asio::streambuf>, std::string>> &evq); //eShrug
	std::string chn;
	std::shared_ptr<asio::ip::tcp::socket> sock;
	unsigned int messageCount = 0;
	void read();
	EventQueue<std::pair<std::unique_ptr<asio::streambuf>, std::string>> &eventQueue;
	std::atomic<bool> quit(false);
	bool sendMsg(const std::string &msg);
	void ping();
private:
	std::atomic<bool> pingReplied(false);
	std::chrono::high_resolution_clock::time_point lastMessageTime;
};

#endif