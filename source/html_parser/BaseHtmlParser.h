#pragma once

#include <vector>
#include <stack>
#include <IHtmlParser.h>
#include <HtmlTagData.h>

namespace Html
{
    class BaseParser : public IParser
    {
    public:
        void ParseHtml(const char *htmlCode) override;

    private:
        void AddTagData(const Tag::Data& tagData);
        void FinalizeTagParsing(const size_t index);
        void ParseTagData(Tag::Data& tagData);
        void ParseTagInfo(Tag::Data& tagData);

        std::vector<Tag::Data> mTagsData;
        std::stack<std::reference_wrapper<Tag::Data>> mHtmlTagsStack;

        Tag::Types mLastTagType = Tag::Types::Comment;
    };
}
