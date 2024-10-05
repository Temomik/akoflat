#pragma once

#include <string>

namespace Url
{
    using std::string;
    
    class IBuilder
    {
    public:
        virtual void SetUrl(const string &url) = 0;
        virtual void AddQuerryItem(const string &item) = 0;
        virtual void AddSubPathItem(const std::string& item) = 0; 
        virtual void AddAnchorItem(const std::string& item) = 0; 
        virtual void Reset() = 0;
        virtual const string Build() const = 0;
    };
}
