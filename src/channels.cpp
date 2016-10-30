#include "channels.hpp"

void Channels::readChannels()
{
	channelsFile.open("channels.txt", std::ios::in);
	if(!channelsFile.is_open())
	{
		channelsFile.open("channels.txt", std::ios::out);
		return;
	}
	std::string line;
	while(std::getline(channelsFile, line))
	{
		if(channelsItemsMap.count(line) == 1) continue;
		std::cout << "reading: " << line << std::endl;
		channelsItemsMap.emplace(line, ChannelItems(line));
	}
	channelsFile.close();
	channelsFile.open("channels.txt", std::ios::out | std::ios::app);
}

void Channels::addChannel(const std::string& channel)
{
	//if(!channelsFile.is_open())
	//{
	//	readChannels();
	//}
	if(channelsItemsMap.count(channel) == 1) return;
	//channelsFile << channel << std::endl;
	ChannelItems *chin = new ChannelItems(channel);
	channelsItemsMap.emplace(channel, std::move(*chin));
	chin->mtx = nullptr;
	delete chin;
}
