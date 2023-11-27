#pragma once

#include "CommandsDeque.h"
#include "IThreadPool.h"
#include "Thread.h"
#include <memory>

#include <deque>

typedef std::shared_ptr<Thread> ThreadPtr;

template <class T>
class ThreadPool : public IThreadPool
{
public:
    ThreadPool(size_t threadsCount = 10, std::chrono::milliseconds waitTime = 300ms)
        : mThreadsCount(threadsCount), mWaitTime(waitTime)
    {
    }

    void AddCommand(std::function<void()> command) override
    {
        mCommands.Push(command);
    }
    
    void Initialize() override
    {
        for (size_t i = 0; i < mThreadsCount; i++)
        {
            auto thread = std::make_shared<Thread>(mCommands, mWaitTime);
            thread->Start();
            mThreads.push_back(thread);
        }
    }

private:
    size_t mThreadsCount;
    std::deque<ThreadPtr> mThreads;
    std::chrono::milliseconds mWaitTime;
    T mCommands;
};
