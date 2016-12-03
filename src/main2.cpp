#include "connhandler.hpp"

int
main(int argc, char *argv[])
{
    ConnHandler irc(argv[1], argv[2]);

    irc.joinChannel("pajlada");
    irc.joinChannel("hemirt");
    irc.joinChannel("forsenlol");

    std::cout << "added all" << std::endl;
    irc.run();
    std::cout << "ended running" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "zulul" << std::endl;

    return 0;
}
