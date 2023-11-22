#pragma once

#include <memory>

namespace Html
{
    class IParser
    {
    public:
        virtual bool ParseHtml(std::shared_ptr<const std::string> htmlCode) = 0;
    };
}
