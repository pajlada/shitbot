#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include <iostream>
#include "channelitems.hpp"
#include <map>
#include <fstream>
#include <memory>
#include <set>

class Channels
{
public:
	std::map<std::string, ChannelItems> channelsItemsMap;
	void readChannels();
	void addChannel(const std::string&);
private:
	std::fstream channelsFile;
	
};

#endif