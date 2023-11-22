#pragma once

#include <memory>
#include <string>

namespace Html
{
    class IRequester
    {
        virtual std::shared_ptr<const std::string> Get(const std::string url, size_t retryCount = 3) = 0;
    };
}
