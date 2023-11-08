#pragma once

#include "IThreadPool.h"
#include "Thread.h"
#include "CommandsDeque.h"
#include <memory>

#include <deque>

typedef std::shared_ptr<Thread> ThreadPtr;

class ThreadPool : public IThreadPool
{
public:
    ThreadPool(size_t threadsCount = 10);

    void AddCommand(std::function<void()> command) override;
    void Initialize() override;
private:

    size_t mThreadsCount;
    std::deque<ThreadPtr> mThreads;
    CommandDeque mCommands;
};
