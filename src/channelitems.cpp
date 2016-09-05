#include "channelitems.hpp"

ChannelItems::ChannelItems(const std::string& channel)
{
	mtx = new std::mutex();
	m_channel = channel;
	if(readFile())
	{
		writeFile();
	}
}

void ChannelItems::insert(const std::string& username)
{
	usersMap[username];
	return;
}
	
void ChannelItems::insert(const std::string& username, const std::string& key, unsigned long long value)
{
	usersMap[username][key] = value;
	return;
}

std::pair<bool, unsigned long long> ChannelItems::get(const std::string& username, const std::string& key)
{
	std::map<std::string, unsigned long long>::iterator it = usersMap[username].find(key);
	if(it != usersMap[username].end())
	{
		return {true, it->second};
	}
	else
	{
		return {false, it->second};
	}
}

int ChannelItems::writeFile()
{
	std::lock_guard<std::mutex> lk(*mtx);
	std::fstream userItemsFile;
	userItemsFile.open(m_channel, std::ios::binary | std::ios::out | std::ios::trunc);
	if(!userItemsFile) 
	{
		return 1;
	}
	for(const auto& i : usersMap)
	{
		//write the length of the username and then the username
		size_t name_size = i.first.size();
		userItemsFile.write(reinterpret_cast<char *>(&name_size), sizeof(size_t));
		userItemsFile.write(i.first.c_str(), name_size);
		//write the number of items the user has
		size_t itemscount = i.second.size();
		userItemsFile.write(reinterpret_cast<char *>(&itemscount), sizeof(size_t));
		//write each item's lenght, then the itemname, then the number of the item the user has
		for(auto y : i.second)
		{
			size_t item_size = y.first.size();
			userItemsFile.write(reinterpret_cast<char *>(&item_size), sizeof(size_t));
			userItemsFile.write(y.first.c_str(), item_size);
			userItemsFile.write(reinterpret_cast<char *>(&y.second), sizeof(unsigned long long));
		}
	}
	userItemsFile.close();
	return 0;
}

int ChannelItems::readFile()
{
	std::lock_guard<std::mutex> lk(*mtx);
	std::fstream userItemsFile;
	userItemsFile.open(m_channel, std::ios::binary | std::ios::in);
	if(!userItemsFile) 
	{
		return 1;
	}
	usersMap.clear();
	while(true)
	{
		//read name length or break on end of file (or some error)
		size_t name_size;
		if(!(userItemsFile.read(reinterpret_cast<char *>(&name_size), sizeof(size_t)))) break;
		//read name to char array
		char * name = new char[name_size];
		userItemsFile.read(name, name_size);
		//make string from name, making it here, cause when i made it near the std::string it in the for loop i got some runtime errors(not sure why LUL)
		std::string nm(name, name_size);
		//read how many items does the user have
		unsigned int itemscount;
		userItemsFile.read(reinterpret_cast<char *>(&itemscount), sizeof(size_t));
		//read each item
		for(int i = 0; i < itemscount; ++i)
		{
			//read length of the item name
			size_t itemsize;
			userItemsFile.read(reinterpret_cast<char *>(&itemsize), sizeof(size_t));
			//read the item name
			char * itemname = new char[itemsize];
			userItemsFile.read(itemname, itemsize);
			//read how many 'itemname' does the user has
			unsigned long long itemnumber;
			userItemsFile.read(reinterpret_cast<char *>(&itemnumber), sizeof(unsigned long long));
			//create a string and insert it to the usersMap
			std::string it(itemname, itemsize);
			usersMap[nm][it] = itemnumber;
			//cleanup
			delete[] itemname;
		}
		//cleanup
		delete[] name;
	}
	userItemsFile.close();
	return 0;
}

void ChannelItems::readAll()
{
	for(const auto& i : usersMap)
	{
		for(const auto y : i.second)
		{
			std::cout << i.first << " " << y.first << " " << y.second << std::endl;
		}
	}
}

size_t ChannelItems::size()
{
	return usersMap.size();
}