#pragma once

#include <string>
#include <vector>

namespace Telegram
{
    class FlatDto
    {
    public:
        std::string Link;
        std::string Price;
        std::string Address;
        std::string Info;
        std::vector<std::string> Images;
    };
}
