#pragma once

#include <map>
#include <queue>
#include <string_view>
#include "HtmlTagType.h"
#include <memory>

namespace Html
{
    namespace Tag
    {

        class Data
        {
        public:
            Data()
            {
            }

            Data(const char *pointer, size_t length, const Tag::Type type)
            {
                raw = std::string_view(pointer, length);
                this->type = type;
            };

            Data(const Data& data)
            {
                this->type = data.type;
                this->raw = data.raw;
                this->data = data.data;
                this->tag = data.tag;
                this->attributes = data.attributes;
            };

            std::queue<std::unique_ptr<Data>> childs;

            Tag::Type type;
            std::string_view raw;
            std::string_view data;
            std::string_view tag;
            std::map<std::string_view, std::string_view> attributes;
        };
    }
}