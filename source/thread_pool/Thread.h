#pragma once

#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

#include "ICommandsDeque.h"

using namespace std::chrono_literals;

class Thread
{
public:
    Thread(ICommandsDeque& commands, std::chrono::milliseconds waitTime = 300ms);
    ~Thread();

    bool Start();
    void Join(bool isForce = false);

private:
    void Execute();

    std::shared_ptr<std::thread> mThread;
    std::mutex mMutex;
    ICommandsDeque& mCommands;

    bool mIsForceJoin = false;
    bool mIsJoin = false;
    bool mIsNotified = false;

    std::chrono::milliseconds mWaitTime;
};
