#include <iostream>
#include "asio.hpp"
#include <chrono>
#include <thread>
#include <map>
#include <mutex>
#include <ctime>
#include <set>
#include <fstream>
#include <random>
#include <iomanip>
#include <locale>
#include <condition_variable>
#include <list>
#include "curl/curl.h"
#include <limits>
#include "utilities.hpp"
#include "eventqueue.hpp"

struct Command
{
	std::string who;
	std::string action;
	std::string data;
};

class IrcConnection
{
public:
	IrcConnection();
	~IrcConnection();
	asio::io_service m_io_service;
	std::map<std::string, std::shared_ptr<asio::ip::tcp::socket>> channelSockets;
	void stop();
	bool start(const std::string& pass, const std::string& nick);
	std::map<std::string, std::chrono::high_resolution_clock::time_point> channelTimes;
	void joinChannel(const std::string&);
	void leaveChannel(const std::string&);
	bool sendMsg(const std::string& channel, const std::string& message);
	bool isAdmin(const std::string& user);
	void addAdmin(const std::string& admin);
	void addCmd(const std::string& cmd, std::string msg);
	std::multimap<std::string, Command> commands;
	std::set<std::string> blacklistset;
	int dice()
	{
		return this->dist(this->mt);
	}
	void listenAndHandle(const std::string& channel);
	std::map<std::string, bool> channelBools;
	std::map<std::string, unsigned> channelMsgs;
	std::mutex irc_m;
	bool quit();
	bool quit_m = false;
	std::vector<std::thread> threads;
	void waitEnd()
	{
		std::unique_lock<std::mutex> lock(irc_m);
		this->quit_cv.wait(lock, [this](){return this->quit_m;});
		lock.unlock();
		for(auto& th : this->threads) 
		{
			if(th.joinable())
				th.join();
		}
	}
	EventQueue<std::pair<std::unique_ptr<asio::streambuf>, std::string>> eventQueue;
	void handleCommands(std::string&, const std::string&, std::string&);
	//Channels channels;
	struct Pings
	{
		std::condition_variable cv;
		std::mutex mtx;
		bool pinged = false;
	};
	std::map<std::string, std::unique_ptr<Pings>> pingMap;
	//void IncrementLoop();
	//ItemIncrements itemincr;
	//int buyItem(const std::string& channel, const std::string& username, std::string& what, unsigned long long howmuch);
	//int sellItem(const std::string& channel, const std::string& username, std::string& what, unsigned long long howmuch);
	int writeCommandsToFile();
	int deleteCmd(const std::string& cmd, const std::string& who);
private:
	std::unique_ptr<asio::io_service::work> wrk;
	void run();
	void msgCount();
	void processEventQueue();
	std::fstream commandsFile;
	std::fstream adminsFile;
	std::fstream blacklist;
	std::set<std::string> admins;
	std::random_device rd;
	std::mt19937 mt{this->rd()};
	std::uniform_int_distribution<int> dist{0, 100};
	std::string pass;
	std::string nick;
	asio::ip::tcp::resolver::iterator twitch_it;
	std::condition_variable quit_cv;
	std::chrono::high_resolution_clock::time_point currentTrigger = std::chrono::high_resolution_clock::now();
};