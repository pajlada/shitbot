#ifndef CONNHANDLER_HPP
#define CONNHANDLER_HPP

#include "asio.hpp"
#include <memory>
#include <map>
#include <string>

class ConnHandler
{
public:
	ConnHandler();
	~ConnHandler();
	std::shared_ptr<socket> spawnSocket();
	std::map<std::string, Channel> channelSockets;
	void joinChannel(const std::string&);
	void leaveChannel(const std::string&);
	void run();
	void stop();
private:
	io_service io_s;
	std::unique_ptr<asio::io_service::work> dummywork;
};

#endif