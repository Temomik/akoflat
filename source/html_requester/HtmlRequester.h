#pragma once

#include "IHtmlRequester.h"

namespace Html
{
    class HtmlRequester : public IRequester
    {
    public:
        HtmlRequester() = default;

        std::shared_ptr<const std::string> Get(const std::string url, size_t retryCount = 3) override;

    private:
    };
}
