#include "channels.hpp"

void Channels::readChannels()
{
	channelsFile.open("channels.txt", std::fstream::in | std::fstream::out);
	if(!channelsFile)
	{
		channelsFile.open("channels.txt", std::fstream::trunc | std::fstream::out);
		channelsFile.open("channels.txt", std::fstream::in | std::fstream::out);
	}
	std::string line;
	while(std::getline(channelsFile, line))
	{
		channelsItemsMap.insert({line, ChannelItems(line)});
	}
	channelsFile.clear();
	channelsFile.seekp(0, std::ios::end);
}

void Channels::addChannel(const std::string& channel)
{
	if(!channelsFile)
	{
		readChannels();
	}
	if(channelsItemsMap.count(channel) == 1) return;
	channelsFile.clear();
	channelsFile.seekp(0, std::ios::end);
	channelsFile << channel << std::endl;
	channelsItemsMap.insert({channel, ChannelItems(channel)});
}