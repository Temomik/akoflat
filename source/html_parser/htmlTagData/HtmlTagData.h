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
            Data() = default;

            Data(const std::string_view& tag, const std::string_view& data, size_t length, const Tag::Type type)
            {
                raw = std::string_view(tag.data(), length);
                this->type = type;
                this->tag = tag;
                this->data = data;
            };

            Data(const std::string_view& tag, size_t length, const Tag::Type type)
            {
                raw = std::string_view(tag.data(), length);
                this->type = type;
                this->tag = tag;
            };

            Data(const Data& data)
            {
                this->type = data.type;
                this->raw = data.raw;
                this->data = data.data;
                this->tag = data.tag;
                this->attributes = data.attributes;
            };

            std::deque<std::shared_ptr<Data>> childs;

            Tag::Type type;
            std::string_view raw;
            std::string_view data;
            std::string_view tag;
            std::map<std::string_view, std::string_view> attributes;
        };
    }
}