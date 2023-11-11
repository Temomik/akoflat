#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadsCount)
    : mThreadsCount(threadsCount)
{
}

void ThreadPool::AddCommand(std::function<void()> command)
{
    mCommands.Push(command);
}

void ThreadPool::Initialize()
{
    for (size_t i = 0; i < mThreadsCount; i++)
    {
        auto thread = std::make_shared<Thread>(mCommands);
        thread->Start();
        mThreads.push_back(thread);
    }
}
