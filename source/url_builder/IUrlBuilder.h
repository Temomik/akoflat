#pragma once

#include <string>

namespace UrlBuilder
{
    using std::string;
    
    class IUrlBuilder
    {
    public:
        virtual void SetUrl(const string &url) = 0;
        virtual void AddQuerryItem(const string &item) = 0;
        virtual void Reset() = 0;
        virtual const string Build() const = 0;
    };
}
