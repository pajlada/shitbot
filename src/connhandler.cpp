#include "connhandler.hpp"

ConnHandler::ConnHandler(const std::string &pss, const std::string &nck) : pass{pss}, nick{nck}
{
	asio::ip::tcp::resolver resolver(io_s);
	asio::ip::tcp::resolver::query query("irc.chat.twitch.tv", "6667");
	twitch_it = resolver.resolve(query);
	
	auto lambda = [this]
	{
		if(!(this->quit())) return;
		for(auto &i : channelSockets)
		{
			if(i.second.messageCount > 0)
				--i.second.messageCount;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	};
	auto thread = std::thread(lambda);
	thread.detach();
}
	
void ConnHandler::spawnSocket(std::string chn)
{
	if(channelSockets.count(chn) == 1) return;
	
	std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(io_s));
	channelSockets.emplace(chn, Channel(chn, sock, eventQueue));
	asio::connect(*sock, twitch_it);
	
	std::string passx = "PASS " + pass + "\r\n";
	std::string nickx = "NICK " + nick + "\r\n";
	std::string cmds = "CAP REQ :twitch.tv/commands\r\n";
	std::string join = "JOIN #" + chn + "\r\n";
	
	sock->send(asio::buffer(passx));
	sock->send(asio::buffer(nickx));
	sock->send(asio::buffer(cmds));	
	sock->send(asio::buffer(join));
	
	//this->channelTimes.insert(std::pair<std::string, std::chrono::high_resolution_clock::time_point>(chn, std::chrono::high_resolution_clock::now()));
	//this->channelMsgs.insert(std::pair<std::string, unsigned>(chn, 0));
	std::cout << "joined" << chn << std::endl;
	
	//std::unique_ptr<Pings> ptr(new Pings);
	//this->pingMap.insert(std::pair<std::string, std::unique_ptr<Pings>>(chn, std::move(ptr)));
	//this->threads.push_back(std::thread(&IrcConnection::listenAndHandle, this, chn)); //start listening on socket
	//std::cout << "started thread: " << chn << std::endl;
}

void ConnHandler::run()
{
	while(!(this->quit()))
	{
		this->eventQueue.wait();
		while(!(this->eventQueue.empty()) && !(this->quit()))
		{
			//std::cout << "event" << std::endl;
			auto pair = this->eventQueue.pop();
			std::unique_ptr<asio::streambuf> b(std::move(pair.first));
			std::string chn = pair.second;
			
			std::istream is(&(*b));
			std::string line(std::istreambuf_iterator<char>(is), {});
			
			std::string delimiter = "\r\n";
			std::vector<std::string> vek;
			size_t pos = 0;
			while((pos = line.find(delimiter)) != std::string::npos)
			{
				vek.push_back(line.substr(0, pos));
				line.erase(0, pos + delimiter.length());
			}
			
			for(int i = 0; i < vek.size(); ++i)
			{
				
				std::string oneline = vek.at(i);
			
				if(oneline.find("PRIVMSG") != std::string::npos)
				{
					size_t pos = oneline.find("PRIVMSG #") + strlen("PRIVMSG #");		
					std::string channel = oneline.substr(pos, oneline.find(":", oneline.find("PRIVMSG #")) - pos - 1);
					std::string ht_chn = "#" + channel;
					std::string msg = oneline.substr(oneline.find(":", oneline.find(ht_chn)) + 1, std::string::npos);
					std::string user = oneline.substr(oneline.find(":") + 1, oneline.find("!") - oneline.find(":") - 1);

					//if(msg[0] == '!')
					{
						//this->handleCommands(user, channel, msg);
					}
				}
				else if(oneline.find("PING") != std::string::npos)
				{
					if(this->channelSockets.count(chn) == 1)
					{
						std::cout << "PONGING" << chn << std::endl;
						std::string pong = "PONG :tmi.twitch.tv\r\n";
						//this->channelSockets[chn].sock->async_send(asio::buffer(pong), handler); //define hnadler
					}
				}
				/*else if(oneline.find("PONG") != std::string::npos)
				{
					{
						std::cout << "got ping " << chn << std::endl;
						std::lock_guard<std::mutex> lk(this->pingMap[chn]->mtx);
						this->pingMap[chn]->pinged = true;
					}
					this->pingMap[chn]->cv.notify_all();
				}
				*/
			}			
		}
	}
}
