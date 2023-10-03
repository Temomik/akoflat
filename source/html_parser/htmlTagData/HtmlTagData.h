#pragma once

#include <vector>
#include <string_view>
#include "HtmlTagTypes.h"
#include "HtmlTagInfo.h"

namespace Html
{
    namespace Tag
    {
        using Html::Data::Unit;

        class Data
        {
        public:
            Data(const char *pointer, size_t length, Tag::Types type)
            {
                raw = string_view(pointer, length);
                this->type = type;
            };

            std::string_view raw;
            std::string_view data;
            Tag::Types type;
            std::vector<Data> childs;
            Info info;
        };
    }
}