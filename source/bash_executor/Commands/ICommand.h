#pragma once

#include "string"

namespace Commands
{
    class ICommand
    {
    public:
        virtual std::string Execute() = 0;
    };
}
