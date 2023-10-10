#pragma once

#include <string>
#include <string_view>

namespace Html
{
    namespace Tag
    {
        enum Type
        {
            Comment = 0,
            Unclosed,
            Closed,
            Script,
            NoScript,
            Style,
            Code
        };
    }
}
