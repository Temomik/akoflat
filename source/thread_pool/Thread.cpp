#include "Thread.h"

#include "CommandsDeque.h"

Thread::Thread(CommandDeque& commands, std::chrono::milliseconds waitTime)
    : mCommands(commands)
    , mWaitTime(waitTime)
{
}

Thread::~Thread()
{
    Join(true);
}

bool Thread::Start()
{
    if (!mThread)
    {
        mIsForceJoin = false;
        mIsJoin = false;

        mThread = std::make_shared<std::thread>([this]{ Execute(); });

        return true;
    }

    return false;
}

void Thread::Join(bool isForce)
{
    if (mThread)
    {
        mIsForceJoin = isForce;
        mIsJoin = true;

        mThread->join();
        mThread.reset();
    }
}

void Thread::Execute()
{
    auto& conditionalVariable = mCommands.GetConditionalVariable();
    
    while(!mIsJoin)
    {
        std::unique_lock lock(mMutex);

        conditionalVariable.wait_for(lock, mWaitTime, [this]{return mIsForceJoin || !mCommands.IsEmpty();});

        if (mIsForceJoin)
        {
            return;
        }

        auto command = mCommands.Pop();

        if (command.has_value())
        {
            command.value()();
        }
    }
}
