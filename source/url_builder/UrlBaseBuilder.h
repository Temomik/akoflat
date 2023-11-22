#pragma once

#include <string>
#include <vector>
#include <IUrlBuilder.h>

namespace Url
{
    class BaseBuilder : public IBuilder
    {
        public:
            void SetUrl(const std::string& url) override; 
            void AddQuerryItem(const std::string& item) override; 
            void AddAnchorItem(const std::string& item) override; 
            void Reset() override;
            const std::string Build() const override;

        protected:
            const char Slash = '/';

            std::string mUrl;
            std::vector<std::string> mQuerryItems;
            std::vector<std::string> mAnchorItems;
    };
}
