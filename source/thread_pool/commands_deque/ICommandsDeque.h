#pragma once

#include <functional>
#include <optional>
#include <condition_variable>

//TODO create base class

class ICommandsDeque
{
public:
    virtual void Push(std::function<void()> command) = 0;
    virtual std::optional<std::function<void()>> Pop() = 0;
    virtual bool IsEmpty() const = 0;
    virtual std::condition_variable& GetConditionalVariable() = 0;
};
