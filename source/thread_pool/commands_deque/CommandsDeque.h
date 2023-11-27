#pragma once

#include <deque>
#include <mutex>

#include "ICommandsDeque.h"

class CommandsDeque : public ICommandsDeque
{
public:
    void Push(std::function<void()> command) override
    {
        std::lock_guard<std::mutex> lock(mMutex);

        mDeque.push_back(command);
    }

    std::optional<std::function<void()>> Pop() override
    {
        std::lock_guard<std::mutex> lock(mMutex);

        if (mDeque.size() == 0)
        {
            return {};
        }

        auto front = mDeque.front();
        mDeque.pop_front();

        return front;
    }

    bool IsEmpty() const override
    {
        std::lock_guard<std::mutex> lock(mMutex);

        return mDeque.empty();
    }

    std::condition_variable& GetConditionalVariable() override
    {
        return mConditionalVariable;
    }

private:
    std::deque<std::function<void()>> mDeque;
    mutable std::mutex mMutex;
    std::condition_variable mConditionalVariable;
};
