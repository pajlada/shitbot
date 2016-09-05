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
		std::cout << "reading: " << line << std::endl;
		channelsItemsMap.insert({line, ChannelItems(line)});
	}
	channelsFile.close();
	channelsFile.open("channels.txt", std::ios::out | std::ios::app);
}

void Channels::addChannel(const std::string& channel)
{
	if(!channelsFile.is_open())
	{
		readChannels();
	}
	if(channelsItemsMap.count(channel) == 1) return;
	channelsFile << channel << std::endl;
	channelsItemsMap.insert({channel, ChannelItems(channel)});
}