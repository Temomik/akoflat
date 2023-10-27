#include "HtmlParserUtils.h"

#include "Utils.h"

using namespace Html;

std::deque<HtmlDot> Html::FindAll(const std::string_view& tag, const AttributeMap& attributeMap, const HtmlDot root)
{
    std::deque<std::shared_ptr<const Tag::Data>> res;

    if (!root)
    {
        return res;
    }

    std::vector<std::shared_ptr<const Tag::Data>> dataStack;
    dataStack.push_back(root);

    while (dataStack.size() != 0)
    {
        auto tagData = dataStack.back();
        dataStack.pop_back();

        for (auto child : tagData->childs)
        {
            dataStack.push_back(child);
        }

        if (Utils::EqualsIC(tagData->tag, tag, tag.size()))
        {
            size_t attributeCount = 0;

            for (auto& attribute : attributeMap)
            {
                if (tagData->attributes.size() != 0)
                {
                    size_t d;
                    d = 1;
                }

                auto it = tagData->attributes.find(attribute.first);
                if (it != tagData->attributes.end() &&
                    it->second.size() == attribute.second.size() &&
                    Utils::EqualsIC(it->second, attribute.second, attribute.second.size()))
                {
                    attributeCount++;
                }
            }

            if (attributeCount == attributeMap.size())
            {
                res.push_back(tagData);
            }
        }
    }

    return res;
}