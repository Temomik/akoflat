#pragma once

#include "HtmlTagType.h"
#include "Utils.h"
#include <string_view>

namespace Html
{
    namespace Tag
    {
        static const char StartTag = '<';
        static const char EndTag = '>';
        static const char ClosedTagSlash = '/';

        static const std::string_view UnclosedTags[] = {
            "<!doctype", "<area",  "<base",   "<br",     "<col",  "<embed",
            "<hr",       "<img",   "<input",  "<keygen", "<link", "<menuitem",
            "<meta",     "<param", "<source", "<track",  "<wbr"};

        Type GetTagType(const std::string_view& tag);
        bool IsEndOfTag(const std::string_view& tag, const std::string_view& endTag);
    }
}
