#pragma once
#include <cstddef>
#include <string_view>

namespace Html
{
    namespace Tag
    {
        class Attribute
        {
        public:
            std::string_view key;
            std::string_view value;
        };
    }
}
