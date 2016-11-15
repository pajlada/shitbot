#ifndef CONNHANDLER_HPP
#define CONNHANDLER_HPP

#include "asio.hpp"
#include <memory>
#include <map>
#include <string>
#include "channel.hpp"
#include <iostream>
#include "eventqueue.hpp"

class ConnHandler
{
public:
	ConnHandler(const std::string &pass, const std::string &nick);
	~ConnHandler();
	std::map<std::string, Channel> channelSockets;
	void joinChannel(const std::string&);
	void leaveChannel(const std::string&);
	void run();
	bool quit();
	EventQueue<std::pair<std::unique_ptr<asio::streambuf>, std::string>> eventQueue;
private:
	asio::io_service io_s;
	asio::ip::tcp::resolver::iterator twitch_it;
	std::unique_ptr<asio::io_service::work> dummywork;
	std::string pass;
	std::string nick;
};


#endif