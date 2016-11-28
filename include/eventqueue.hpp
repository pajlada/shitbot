#ifndef EVENTQUEUE_HPP
#define EVENTQUEUE_HPP

#include "asio.hpp"

#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class EventQueue {
    std::list<T> _queue;
    std::condition_variable cv;
    std::mutex m;
    std::unique_lock<std::mutex> lk;
 
public:
    void push(T t) {
        std::unique_lock<std::mutex> lk(m);
        _queue.push_back(move(t));
        lk.unlock();
        cv.notify_all();
    }
	
	void notify() {
        cv.notify_all();
    }
 
    T pop() {
        std::unique_lock<std::mutex> lk(m);
        T value = std::move(_queue.front());
        _queue.pop_front();
        lk.unlock();
        return value;
    }
 
    void wait() {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk);
    }
 
    template<typename Lambda>
    void wait(Lambda f) {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk);
        lk.unlock();
        f();
    }
 
    bool empty() {
        std::unique_lock<std::mutex> lk(m);
        bool e = _queue.empty();
        lk.unlock();
        return e;
    }
 
    size_t size() {
        std::unique_lock<std::mutex> lk(m);
        size_t s = _queue.size();
        lk.unlock();
        return s;
    }
};

typedef EventQueue<std::pair<std::unique_ptr<asio::streambuf>, std::string>> BotEventQueue;

#endif