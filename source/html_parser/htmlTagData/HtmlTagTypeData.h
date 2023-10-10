#pragma once
#include "HtmlTagType.h"

namespace Html
{
    namespace Tag
    {
        template <Type type>
        struct TypeData
        {
            static constexpr std::string_view TagStart = "~~~~";
            static constexpr std::string_view TagEnd = "~~~~~";
        };

        template <>
        struct TypeData<Type::Comment>
        {
            static constexpr std::string_view TagStart = "<!--";
            static constexpr std::string_view TagEnd = "-->";
        };

        template <>
        struct TypeData<Type::Script>
        {
            static constexpr std::string_view TagStart = "<script";
            static constexpr std::string_view TagEnd = "</script";
        };

        template <>
        struct TypeData<Type::Style>
        {
            static constexpr std::string_view TagStart = "<style";
            static constexpr std::string_view TagEnd = "</style";
        };

        template <>
        struct TypeData<Type::Code>
        {
            static constexpr std::string_view TagStart = "<code";
            static constexpr std::string_view TagEnd = "</code";
        };

        template <>
        struct TypeData<Type::NoScript>
        {
            static constexpr std::string_view TagStart = "<noscript";
            static constexpr std::string_view TagEnd = "</noscript";
        };
    }
}
