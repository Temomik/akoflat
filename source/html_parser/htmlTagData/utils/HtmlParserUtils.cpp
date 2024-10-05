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

        for (const auto& child : tagData->childs)
        {
            dataStack.push_back(child);
        }

        if (Utils::EqualsIC(tagData->tag, tag, tag.size(), 1)) // 1 offset is need to skip <
        {
            size_t attributeCount = 0;

            for (const auto& attribute : attributeMap)
            {
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

bool Html::ExportData(std::string& value, const std::string_view& tag, const AttributeMap& attributeMap, const HtmlDot root)
{
    auto dataDot = FindAll(tag, attributeMap, root);

    if (dataDot.size() == 1)
    {
        value = dataDot.front()->data;

        return true;
    }

    return false;
}

bool Html::ExportDataArray(std::vector<std::string>& array, const std::string_view& tag, const AttributeMap& attributeMap, const HtmlDot root)
{
    auto dataDot = FindAll(tag, attributeMap, root);
    bool status = false;

    if (dataDot.size() > 0)
    {
        for (const auto& dot : dataDot)
        {
            array.push_back(std::string(dot->data));
            status = true;
        }
    }

    return status;
}

bool Html::ExportAttributeValue(std::string& value, const std::string key, const std::string_view& tag, const AttributeMap& attributeMap, const HtmlDot root)
{
    auto dataDot = FindAll(tag, attributeMap, root);

    if (dataDot.size() == 1)
    {
        auto attributeDot = dataDot.front()->attributes.find(key);

        if (attributeDot != dataDot.front()->attributes.end())
        {
            value = (*attributeDot).second;

            return true;
        }
    }

    return false;
}

bool Html::ExportAttributeValueArray(std::vector<std::string>& value, const std::string key, const std::string_view& tag, const AttributeMap& attributeMap, const HtmlDot root)
{
    auto dataDot = FindAll(tag, attributeMap, root);
    bool status = false;

    if (dataDot.size() > 0)
    {

        for (const auto& data : dataDot)
        {
            auto attributeDot = data->attributes.find(key);

            if (attributeDot != data->attributes.end())
            {
                auto attributeData = std::string((*attributeDot).second);
                value.push_back(attributeData);
                status = true;
            }
        }
    }

    return status;
}
