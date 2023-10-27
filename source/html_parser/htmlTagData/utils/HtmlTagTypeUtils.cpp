#include "HtmlTagTypeUtils.h"
#include "HtmlTagTypeData.h"
#include <cctype>

using Html::Tag::Type;
using Html::Tag::TypeData;

// TODO refactor
Type Html::Tag::GetTagType(const std::string_view& tag)
{
    for (auto& unclosedTag : UnclosedTags)
    {
        if (Utils::FindIC(tag, unclosedTag, tag.length()) != std::string_view::npos)
        {
            return Type::Unclosed;
        }
    }

    if (Utils::FindIC(tag, TypeData<Type::Comment>::TagStart, tag.length()) !=
        std::string_view::npos)
    {
        return Type::Comment;
    }

    if (Utils::FindIC(tag, TypeData<Type::Script>::TagStart, tag.length()) !=
        std::string_view::npos)
    {
        return Type::Script;
    }

    if (Utils::FindIC(tag, TypeData<Type::Style>::TagStart, tag.length()) != std::string_view::npos)
    {
        return Type::Style;
    }

    if (Utils::FindIC(tag, TypeData<Type::Code>::TagStart, tag.length()) != std::string_view::npos)
    {
        return Type::Code;
    }

    if (Utils::FindIC(tag, TypeData<Type::NoScript>::TagStart, tag.length()) !=
        std::string_view::npos)
    {
        return Type::NoScript;
    }

    return Type::Closed;
}

bool Html::Tag::IsEndOfTag(const std::string_view& tag, const std::string_view& endTag)
{
    constexpr size_t minTagLength = 3;

    if (endTag.length() >= minTagLength && tag.length() >= minTagLength)
    {
        if (endTag[1] == Tag::ClosedTagSlash)
        {
            auto len = tag.find_first_of(" \n\t>") - 1;

            // +1 and +2 are used to get rid of < and </ tag's part
            auto clearTag = std::string_view(tag.data() + 1, len);
            auto clearEndTag = std::string_view(endTag.data() + 2, len);

            auto pos = Utils::FindIC(clearEndTag, clearTag, len);

            return pos != std::string_view::npos;
        }
    }

    return false;
}

bool Html::Tag::IsSeparatorCharacter(const char character)
{
    return character == Tag::EndTag || std::isspace(character);
}
