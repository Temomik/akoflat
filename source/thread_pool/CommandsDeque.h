#pragma once 

#include <mutex>
#include <deque>
#include <functional>
#include <condition_variable>
#include <optional>

class CommandDeque
{
public:
    void Push(std::function<void()> command)
    {
        std::lock_guard<std::mutex> lock(mMutex);

        mDeque.push_back(command);
    }

    std::optional<std::function<void()>> Pop()
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

    bool IsEmpty() const
    {
        std::lock_guard<std::mutex> lock(mMutex);

        return mDeque.empty();
    }
    
    std::condition_variable& GetConditionalVariable()
    {
        return mConditionalVariable;
    }
    
private:
    std::deque<std::function<void()>> mDeque;
    mutable std::mutex mMutex;
    std::condition_variable mConditionalVariable;
};
