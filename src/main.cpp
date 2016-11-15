#include "ircconnection.hpp"

int main(int argc, char *argv[])
{
	IrcConnection myIrc;
	while(!myIrc.start(argv[1], argv[2]))
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	
	myIrc.joinChannel("pajlada");
	myIrc.joinChannel("hemirt");
	myIrc.joinChannel("forsenlol");

	std::cout << "added all" << std::endl;
	myIrc.waitEnd();
	return 0;
}