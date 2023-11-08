#pragma once

#include <functional>

class IThreadPool
{
public:
    virtual void AddCommand(std::function<void()> command) = 0;
    virtual void Initialize() = 0;
};
