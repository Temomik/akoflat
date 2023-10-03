#include <BaseHtmlParser.h>
#include <stddef.h>
#include <iostream>
#include <string>
#include <fstream>
#include <string_view>

using std::string;
using std::string_view;
using namespace Html;

void BaseParser::ParseHtml(const char *htmlCode)
{

    std::ofstream output;

    output.open("parsed.html");
    for (size_t i = 0; htmlCode[i]; i++)
    {
        if (htmlCode[i] == Tag::StartTag)
        {
            if (mLastTagType == Tag::Types::Closed && htmlCode[i + 1] == Tag::ClosedTagSlash)
            {
                while (htmlCode[i] && htmlCode[i] != Tag::EndTag)
                {
                    i++;
                }
                
                //TODO handle invalid html file when htmlCode[i] != Tag::EndTag
                FinalizeTagParsing(i);

                continue;
            };

            auto tagType = Tag::GetTagType(htmlCode + i);
            auto tagData = Tag::Data(htmlCode + i, i, tagType);

            // skip script
            if (tagType == Tag::Types::Script)
            {
                string_view scriptTag(htmlCode + i);
                string endTag = "</script";

                auto endScriptIndex = scriptTag.find(endTag);

                i += endScriptIndex + endTag.size();
                continue;
            }

            // skip comment
            if (tagType == Tag::Types::Comment)
            {
                string_view scriptTag(htmlCode + i);
                string endTag = "-->";

                auto endIndex = scriptTag.find(endTag);

                i += endIndex + endTag.size();
                continue;
            }

            // skip style
            if (tagType == Tag::Types::Style)
            {
                string_view scriptTag(htmlCode + i);
                string endTag = "</style";

                auto endIndex = scriptTag.find(endTag);

                i += endIndex + endTag.size();
                continue;
            }

            AddTagData(tagData);
            continue;
        }

        if (htmlCode[i] == Tag::EndTag)
        {
            if (mLastTagType == Tag::Types::Closed)
            {
                continue;
            }

            FinalizeTagParsing(i);

            continue;
        }
    }
    output.close();
}

void BaseParser::AddTagData(const Tag::Data &tagData)
{
    mLastTagType = tagData.type;

    if (mHtmlTagsStack.size() == 0)
    {
        mTagsData.push_back(tagData);
        mHtmlTagsStack.push(mTagsData.back());
    } else 
    {
        auto& lastTagChilds = mHtmlTagsStack.top().get().childs;
        lastTagChilds.push_back(tagData);
        mHtmlTagsStack.push(lastTagChilds.back());
    }
}

void BaseParser::FinalizeTagParsing(const size_t index)
{
    // TODO add parsing of inner data of tag;
    if (mHtmlTagsStack.size() > 0)
    {
        auto &tagData = mHtmlTagsStack.top().get();

        tagData.raw.length = index - tagData.raw.length + 1;
        
        ParseTagData(tagData);
        ParseTagInfo(tagData);

        mHtmlTagsStack.pop();
    }

    if (mHtmlTagsStack.size() > 0)
    {
        mLastTagType = mHtmlTagsStack.top().get().type;
    } else
    {
        mLastTagType = Tag::Types::Comment;
    }
}

void BaseParser::ParseTagData(Tag::Data &tagData)
{
    if (tagData.type == Tag::Types::Closed)
    {
        size_t startIndex = 0, endIndex = 0;

        for (size_t i = 0; i < tagData.raw.length; i++)
        {
            if (tagData.raw.pointer[i] == Tag::EndTag)
            {
                startIndex = i + 1;
                break;
            }
        }

        for (size_t i = startIndex; i < tagData.raw.length; i++)
        {
            if (tagData.raw.pointer[i] == Tag::StartTag)
            {
                endIndex = i;
                break;
            }
        }

        tagData.data.pointer = tagData.raw.pointer + startIndex;
        tagData.data.length = endIndex - startIndex;

        string tmp(tagData.data.pointer, tagData.data.length);
        if (tagData.data.length > 0)
        {
            std::cout << "|__|" << tmp << "|__|" << std::endl;
        } 
    }
}

bool IsSeparatorCharacter(const char character)
{
    switch (character)
    {
    case ' ':
        return true;
    case '\n':
        return true;
    case '\t':
        return true;
    case Tag::EndTag:
        return true;
    default:
        return false;
    }
}

bool IsAttributeSeparator(const char* data)
{
    return strncmp("=\"", data, 2) == 0;
}


//todo fix  class="gb_nd gb_xd gb_td gb_Be gb_Oe gb_Te"> this data parsing and test it
void BaseParser::ParseTagInfo(Tag::Data &tagData)
{   
    if (tagData.type == Tag::Types::Comment)
    {
        return;
    }

    size_t tagAttributesIndex = 1;

    auto &rawPtr = tagData.raw.pointer;

    for (; !IsSeparatorCharacter(rawPtr[tagAttributesIndex]); tagAttributesIndex++)
    {
    }

    // +- need to skip start tag character
    tagData.info.tag.pointer = tagData.raw.pointer + 1;
    tagData.info.tag.length = tagAttributesIndex - 1;

    size_t infoLength = 0;

    for (size_t i = tagAttributesIndex; i < tagData.raw.length; i++)
    {
        size_t startIndex = 0, endIndex = 0, separatorIndex = 0;

        for (; IsSeparatorCharacter(rawPtr[i]); i++)
        {
        };
        startIndex = i;

        for (; !IsSeparatorCharacter(rawPtr[i]); i++)
        {
            if (IsAttributeSeparator(rawPtr + i))
            {
                separatorIndex = i;
            }
        };

        endIndex = i;

        if (separatorIndex != 0)
        {
            Tag::Attribute attribute;

            attribute.key.pointer = tagData.raw.pointer + startIndex;
            attribute.key.length = separatorIndex - startIndex;

            attribute.value.pointer = tagData.raw.pointer + separatorIndex + 1;
            attribute.value.length = endIndex - startIndex;
             

            tagData.info.attributes.push_back(attribute);
        }

        if (rawPtr[i] != Tag::EndTag )
        {
            break;
        }
    }
}
