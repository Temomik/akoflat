#pragma once

#include <string>
#include <vector>
#include <utility>

namespace Telegram
{
    namespace User
    {
        class Config
        {
        public:
            std::vector<size_t> FloorCount;
            std::vector<std::string> Platforms;
            std::string City;
            std::pair<size_t, size_t> Price;     
        };   
    }
}
