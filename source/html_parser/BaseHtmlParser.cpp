#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <optional>
#include <set>
#include <stddef.h>
#include <string>
#include <utility>

#include "BaseHtmlParser.h"
#include "HtmlTagTypeData.h"
#include "HtmlTagTypeUtils.h"
#include "Utils.h"

using std::string;
using std::string_view;
using namespace Html;

namespace Html
{
    // TODO Split to file
    class TagAssociatedData
    {
    public:
        size_t index;
        std::vector<size_t> childs;
        bool isTagDataCreated = false;
    };
}

bool BaseParser::ParseHtml(std::shared_ptr<const std::string> htmlCodePtr)
{
    mHtmlCode = htmlCodePtr;
    const char* htmlCode = mHtmlCode->c_str();

    string_view codeView(htmlCode);
    std::vector<TagPair> tagPairs;

    std::vector<string_view> tagsArray;

    for (size_t i = 0; i < codeView.size(); i++)
    {
        auto index = codeView.find(Tag::StartTag, i);
        if (index != string_view::npos)
        {
            i = index;
            index = codeView.find_first_of(">", i);
            string_view tag(htmlCode + i, index - i + 1);

            size_t countToSkip = 0;

            countToSkip += GetSkipTagCount<Tag::Type::Comment>(tag, codeView, i);
            countToSkip += GetSkipTagCount<Tag::Type::Script>(tag, codeView, i);
            countToSkip += GetSkipTagCount<Tag::Type::Style>(tag, codeView, i);
            countToSkip += GetSkipTagCount<Tag::Type::Code>(tag, codeView, i);
            countToSkip += GetSkipTagCount<Tag::Type::NoScript>(tag, codeView, i);

            i += countToSkip;

            if (countToSkip == 0)
            {
                tagsArray.push_back(tag);
            }
        }
    }

    for (size_t i = 0; i < tagsArray.size(); i++)
    {
        auto endIndex = ParseTagPair(tagsArray, i, codeView);

        if (endIndex.has_value())
        {
            tagPairs.push_back(TagPair{i, endIndex.value()});
        }
    }

    // TODO sort tagPairs

    std::vector<TagAssociatedData> tagAssociatedData;

    for (size_t i = 0; i < tagPairs.size(); i++)
    {
        TagAssociatedData currentTagAssociatedData;
        currentTagAssociatedData.index = i;

        for (size_t j = i + 1; j < tagPairs.size(); j++)
        {
            if (tagPairs[i].first < tagPairs[j].first && tagPairs[i].second > tagPairs[j].second)
            {
                currentTagAssociatedData.childs.push_back(j);
            }
        }

        tagAssociatedData.push_back(currentTagAssociatedData);
    }

    for (size_t i = 0; i < tagAssociatedData.size(); i++)
    {
        std::set<size_t> indexToRemove;

        for (auto child : tagAssociatedData[i].childs)
        {
            for (auto indexes : tagAssociatedData[child].childs)
            {
                indexToRemove.insert(indexes);
            }
        }

        auto& childs = tagAssociatedData[i].childs;
        childs.erase(std::remove_if(childs.begin(), childs.end(),
                                    [indexToRemove](size_t index)
                                    { return indexToRemove.find(index) != indexToRemove.end(); }),
                     childs.end());
    }

    mDotRoot = std::make_shared<Tag::Data>();
    for (auto& tagData : tagAssociatedData)
    {
        if (tagData.isTagDataCreated)
        {
            continue;
        }

        tagData.isTagDataCreated = true;
        mDotRoot->childs.push_back(CreateTagData(tagPairs, tagsArray, tagData));

        std::deque<std::pair<TagAssociatedData&, std::shared_ptr<Tag::Data>>> tagDataStack;

        auto stackPair = std::make_pair(std::ref(tagData), mDotRoot->childs.back());

        tagDataStack.push_back(stackPair);

        for (; tagDataStack.size() != 0;)
        {
            auto top = tagDataStack.back();
            tagDataStack.pop_back();
            ParseTagInfo(top.second);

            for (auto& child : top.first.childs)
            {
                auto& childAssociatedData = tagAssociatedData[child];
                childAssociatedData.isTagDataCreated = true;

                auto tmp = CreateTagData(tagPairs, tagsArray, childAssociatedData);

                top.second->childs.push_back(tmp);

                auto stackPair = std::make_pair(std::ref(childAssociatedData),
                                                top.second->childs.back());

                tagDataStack.push_back(stackPair);
            }
        }
    }

    // TODO split to functions and add return status handling
    return true;
}

const std::shared_ptr<const Tag::Data> BaseParser::GetHtmlDotRoot() const
{
    return mDotRoot;
}

std::optional<size_t> BaseParser::ParseTagPair(const std::vector<string_view>& tags,
                                               const size_t index, const string_view& codeView)
{
    auto type = Tag::GetTagType(tags[index]);

    if (type == Tag::Type::Unclosed)
    {
        return index;
    }
    else
    {
        auto& tag = tags[index];
        // TODO create method in utils with name IsEndTag
        if (Utils::FindIC(tag, "</", tag.length()) != string_view::npos)
        {
            // skip end tag
            return {};
        }

        size_t sameTagCount = 0;
        for (size_t j = index + 1; j < tags.size(); j++)
        {
            // TODO create method in utils with name GetStartTagLength
            auto length = tag.find_first_of(" \t\n>");

            if (Utils::EqualsIC(tag, tags[j], length))
            {
                sameTagCount++;
                continue;
            }

            if (Tag::IsEndOfTag(tag, tags[j]))
            {
                if (sameTagCount == 0)
                {
                    return j;
                }
                else
                {
                    sameTagCount--;
                }
            }
        }
    }

    return {};
}

std::shared_ptr<Tag::Data> BaseParser::CreateTagData(const std::vector<TagPair>& tagPairs,
                                                     const std::vector<string_view>& tagsArray,
                                                     const TagAssociatedData& associatedData)
{
    auto& tagPair = tagPairs[associatedData.index];
    auto& startTag = tagsArray[tagPair.first];
    auto& endTag = tagsArray[tagPair.second];
    auto size = (endTag.end() - startTag.data()) / sizeof(char);
    auto type = Tag::GetTagType(startTag);

    if (type == Tag::Type::Unclosed)
    {
        size = startTag.size();
        return std::make_shared<Tag::Data>(Tag::Data(startTag, size, type));
    }

    // TODO add new template method in utils with name GetLength<char>()
    auto dataSize = (endTag.begin() - startTag.end()) / sizeof(char);
    auto data = string_view(startTag.end(), dataSize);

    return std::make_shared<Tag::Data>(Tag::Data(startTag, data, size, type));
}

template <Tag::Type type>
size_t BaseParser::GetSkipTagCount(const string_view& tag, const string_view& htmlData,
                                   const size_t offset)
{
    Tag::TypeData<type> tagData;
    auto startIndex = Utils::FindIC(tag, tagData.TagStart, tagData.TagStart.length());
    if (startIndex != string_view::npos)
    {
        auto length = htmlData.length() - offset;
        auto commentEndIndex = Utils::FindIC(htmlData, tagData.TagEnd, length, offset);

        if (commentEndIndex != string_view::npos)
        {
            return commentEndIndex - offset + tagData.TagEnd.size();
        }
    }

    return 0;
}

// TODO move to utils
bool IsAttributeSeparator(const char* data) { return strncmp("=\"", data, 2) == 0; }

// todo tests for attribute parsing
void BaseParser::ParseTagInfo(std::shared_ptr<Tag::Data> tagData)
{
    auto size = tagData->tag.size();
    auto& tag = tagData->tag;

    for (size_t i = 0; i < size; i++)
    {
        while (i < size && !std::isspace(tag[i]))
        {
            i++;
        }

        while (i < size && std::isspace(tag[i]))
        {
            i++;
        }

        size_t keyStart = i;

        while (i < size && tag[i] != '\"')
        {
            i++;
        }
        size_t keyEnd = i;
        i++;

        while (i < size && tag[i] != '\"')
        {
            i++;
        }

        auto keyLength = std::max(0ul, keyEnd - keyStart + 1 - 2);
        auto valueLength = std::max(0ul, i - keyEnd + 1 - 2);

        if (keyLength == 0 || valueLength == 0)
        {
            continue;
        }

        string_view key(tag.data() + keyStart, keyLength);
        string_view value(tag.data() + keyEnd + 1, valueLength);

        tagData->attributes.insert(std::pair(key, value));
    }
}
