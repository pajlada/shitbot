#include "connhandler.hpp"

int main(int argc, char *argv[])
{
	ConnHandler irc(argv[1], argv[2]);

	
	irc.joinChannel("pajlada");
	irc.joinChannel("hemirt");
	irc.joinChannel("forsenlol");

	std::cout << "added all" << std::endl;
	irc.run();
	return 0;
}