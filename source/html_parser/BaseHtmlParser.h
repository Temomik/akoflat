#pragma once

#include "HtmlTagData.h"
#include "IHtmlParser.h"

#include <map>
#include <queue>
#include <stack>
#include <string_view>
#include <vector>

namespace Html
{
    class TagAssociatedData;

    typedef std::pair<size_t, size_t> TagPair;

    class BaseParser : public IParser
    {
    public:
        void ParseHtml(const char* htmlCode) override;
        const std::shared_ptr<const Tag::Data> GetHtmlDotRoot() const;

    private:
        std::optional<size_t> ParseTagPair(const std::vector<std::string_view>& tags,
                                           const size_t index, const std::string_view& codeView);

        std::shared_ptr<Tag::Data> CreateTagData(const std::vector<TagPair>& tagPairs,
                                                 const std::vector<std::string_view>& tagsArray,
                                                 const TagAssociatedData& associatedData);

        template <Tag::Type type>
        size_t GetSkipTagCount(const std::string_view& tag, const std::string_view& htmlData,
                               const size_t offset);

        void ParseTagInfo(std::shared_ptr<Tag::Data> tagData);

        std::shared_ptr<Tag::Data> mDotRoot;
        Tag::Type mLastTagType = Tag::Type::Comment;
    };
} // namespace Html
