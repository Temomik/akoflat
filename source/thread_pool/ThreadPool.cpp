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
        mThreads.push_back(std::make_shared<Thread>(mCommands));
    }
}
