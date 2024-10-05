#pragma once

#include "BaseHtmlParser.h"

#include <string>
#include <memory>

#include "FlatDto.h"

namespace Onliner
{
    class HtmlParser : private Html::BaseParser
    {
    public:
        HtmlParser(std::shared_ptr<const std::string> htmlCode);

        bool Parse(std::vector<Telegram::FlatDto>& flatsDto);

        size_t GetPagesCount() const;
    private:
        size_t mPagesCount = 1;
        std::shared_ptr<const std::string> mHtmlCode;
    };
}
