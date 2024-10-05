#pragma once

#include <deque>
#include <mutex>
#include <ctime>

#include "ICommandsDeque.h"

class DelayedCommandsDeque : public ICommandsDeque
{

public:
    void Push(std::function<void()> command) override
    {
        std::lock_guard<std::mutex> lock(mMutex);

        mDeque.push_back({std::time(0), command});
    }

    std::optional<std::function<void()>> Pop() override
    {
        std::lock_guard<std::mutex> lock(mMutex);

        if (!IsCommandReady())
        {
            return {};
        }

        auto front = mDeque.front();
        mDeque.pop_front();

        return front.second;
    }

    bool IsEmpty() const override
    {
        std::lock_guard<std::mutex> lock(mMutex);

        return !IsCommandReady();
    }

    std::condition_variable& GetConditionalVariable() override
    {
        return mConditionalVariable;
    }

private:
    bool IsCommandReady() const
    {
        return mDeque.size() != 0 && mDeque.front().first + mDelayTime < std::time(0);
    }

    std::deque<std::pair<time_t, std::function<void()>>> mDeque;
    mutable std::mutex mMutex;
    std::condition_variable mConditionalVariable;

    //TODO make this value configurable
    const time_t mDelayTime = 60 * 10; 
};
