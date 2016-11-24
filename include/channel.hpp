#ifndef CHANNEL_HPP
#define CHANNEL_HPP

/// Includes are spread up into three parts
// 1st party files
#include "eventqueue.hpp"

// 3rd party files
#include "asio.hpp"

// stdlib includes
#include <atomic>
#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <string>

typedef EventQueue<std::pair<std::unique_ptr<asio::streambuf>, std::string>> BotEventQueue;

class Channel
{
public:
    Channel(const std::string &_channelName,
            std::shared_ptr<asio::ip::tcp::socket> sock,
            BotEventQueue &evq);

    void read();
    bool sendMsg(const std::string &msg);
    void ping();

    // Channel name (i.e. "pajlada" or "forsenlol)
    std::string channelName;

    // What does the event queue do?
    BotEventQueue &eventQueue;

    // Set to true if the channel should stop reading new messages
    std::atomic<bool> quit;

    //right now public, connhandler is using it
    unsigned int messageCount = 0;
    // Pointer to socket so we can send messages
    std::shared_ptr<asio::ip::tcp::socket> sock;
    
private:
    // XXX: This doesn't even seem to be used
    std::atomic<bool> pingReplied;

    // Used in anti-spam measure so we don't get globally banned
    std::chrono::high_resolution_clock::time_point lastMessageTime;

};

#endif