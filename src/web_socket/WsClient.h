#pragma once
#include "easywsclient.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <deque>
#include <thread>
#include <chrono>
#include <atomic>

// a simple, thread-safe queue with (mostly) non-blocking reads and writes
namespace non_blocking {
    template <class T>
    class Queue { 
        mutable std::mutex m;
        std::deque<T> data;
    public:
        void push(T const &input) { 
            std::lock_guard<std::mutex> L(m);
            data.push_back(input);
        }

        bool pop(T &output) {
            std::lock_guard<std::mutex> L(m);
            if (data.empty())
                return false;
            output = data.front();
            data.pop_front();
            return true;
        }
    };
}

// eastwsclient isn't thread safe, so this is a really simple
// thread-safe wrapper for it.
class WebSocketClient {
public:
    WebSocketClient(const std::string& url);
    ~WebSocketClient();

    inline void Send(std::string const &s) { m_Outgoing.push(s); }
    inline bool Recv(std::string &s) { return m_Incoming.pop(s); }
    inline void Close() { m_Running = false; }
private:
    std::thread m_Runner;                           // Main websocket thread (handles sending and receiving messages)
    non_blocking::Queue<std::string> m_Outgoing;    // Outgoing messages queue
    non_blocking::Queue<std::string> m_Incoming;    // Incoming messages queue
    std::atomic<bool> m_Running { true };           // Flag to stop the main websocket thread
};