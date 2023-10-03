#pragma once

#include <vector>
#include "HtmlTagAttribute.h"
#include <string_view>

namespace Html
{
    namespace Tag
    {
        class Info
        {
        public:
            std::string_view tag;
            std::vector<Attribute> attributes;
        };
    }
}
