#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <memory>
#include <chrono>

class CommandDeque;
using namespace std::chrono_literals;

class Thread
{
public:
    Thread(CommandDeque& commands, std::chrono::milliseconds waitTime = 300ms);
    ~Thread();

    bool Start();
    void Join(bool isForce = false);
private:
    void Execute();

    std::shared_ptr<std::thread> mThread;
    std::mutex mMutex;
    CommandDeque& mCommands;

    bool mIsForceJoin = false;
    bool mIsJoin = false;
    bool mIsNotified = false;

    std::chrono::milliseconds mWaitTime;
};
