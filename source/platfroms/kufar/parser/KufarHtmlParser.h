#pragma once

#include "BaseHtmlParser.h"

#include <string>
#include <memory>

#include "FlatDto.h"

namespace Kufar
{
    class HtmlParser : private Html::BaseParser
    {
    public:
        HtmlParser(std::shared_ptr<const std::string> htmlCode);

        bool Parse(std::vector<Telegram::FlatDto>& flatsDto);

        std::string GetPageLink(const size_t pageCount) const;
    private:
        std::shared_ptr<const std::string> mHtmlCode;
    };
}
