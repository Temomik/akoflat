#pragma once

#include <string>

namespace Html
{
    namespace Tag
    {
        static const char StartTag = '<';
        static const char EndTag = '>';
        static const char ClosedTagSlash = '/';

        static const std::string CommentTagStart = "!--";
        static const std::string CommentTagEnd = "-->";

        static const std::string ScriptTag = "script";
        static const std::string StyleTag = "style";

        static const std::string UnclosedTags[] =
            {
                "!doctype", "area", "base", "br", "col", "embed",
                "hr", "img", "input", "keygen", "link",
                "menuitem", "meta", "param", "source", "track", "wbr",
                "!DOCTYPE", "AREA", "BASE", "BR", "COL", "EMBED",
                "HR", "IMG", "INPUT", "KEYGEN", "LINK",
                "MENUITEM", "META", "PARAM", "SOURCE", "TRACK", "WBR",
                };

        enum Types
        {
            Comment = 0,
            Unclosed,
            Closed,
            Script, 
            Style
        };

        static Types GetTagType(const char *htmlData)
        {
            auto pointer = htmlData + 1; // skip base tag

            for (auto& UnclosedTag : UnclosedTags)
            {
                auto result = std::strncmp(UnclosedTag.c_str(), pointer, UnclosedTag.size());
                if (result == 0)
                {
                    return Types::Unclosed;
                }
            }

            if (std::strncmp(CommentTagStart.c_str(), pointer, CommentTagStart.size()) == 0)
            {
                return Types::Comment;
            }

            if (std::strncmp(ScriptTag.c_str(), pointer, ScriptTag.size()) == 0)
            {
                return Types::Script;
            }

            if (std::strncmp(StyleTag.c_str(), pointer, StyleTag.size()) == 0)
            {
                return Types::Style;
            }

            return Types::Closed;
        }
    }
}
