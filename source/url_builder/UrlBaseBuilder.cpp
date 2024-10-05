#include <UrlBaseBuilder.h>

namespace Url
{
    void BaseBuilder::SetUrl(const string& url)
    {
        mUrl = url;
    }

    void BaseBuilder::AddSubPathItem(const std::string& item)
    {
        mSubPathItems.push_back(item);
    }

    void BaseBuilder::AddQuerryItem(const string& item)
    {
        mQuerryItems.push_back(item);
    }

    void BaseBuilder::AddAnchorItem(const std::string& item)
    {
        mAnchorItems.push_back(item);
    }

    void BaseBuilder::Reset()
    {
        mUrl.clear();
        mQuerryItems.clear();
    }

    const string BaseBuilder::Build() const
    {
        string querry;
        string subPath;
        char separator = '?';

        for (const auto& item : mSubPathItems)
        {
            subPath += Slash + item;
        }

        for (size_t i = 0; i < mQuerryItems.size(); ++i)
        {
            querry += separator;
            querry += mQuerryItems[i];

            separator = '&';
        }

        separator = '#';
        for (size_t i = 0; i < mAnchorItems.size(); ++i)
        {
            querry += separator;
            querry += mAnchorItems[i];

            separator = '&';
        }

        string link = mUrl + subPath + Slash + querry;

        return link;
    }
}
