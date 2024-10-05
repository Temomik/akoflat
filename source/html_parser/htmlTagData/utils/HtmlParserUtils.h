#pragma once

#include <map>
#include <string_view>
#include <deque>

#include "HtmlTagData.h"

namespace Html
{
    typedef std::map<std::string_view, std::string_view> AttributeMap;
    typedef std::shared_ptr<const Tag::Data> HtmlDot;

    std::deque<HtmlDot> FindAll(const std::string_view& tag, const AttributeMap& attributeMap, const HtmlDot root);

    bool ExportData(std::string& value, const std::string_view& tag, const AttributeMap& attributeMap, const HtmlDot root);
    bool ExportDataArray(std::vector<std::string>& array, const std::string_view& tag, const AttributeMap& attributeMap, const HtmlDot root);
   
    bool ExportAttributeValue(std::string& value, const std::string key, const std::string_view& tag, const AttributeMap& attributeMap, const HtmlDot root);
    bool ExportAttributeValueArray(std::vector<std::string>& value, const std::string key, const std::string_view& tag, const AttributeMap& attributeMap, const HtmlDot root);
}