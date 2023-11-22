#include <UrlBaseBuilder.h>

namespace Url
{
    void BaseBuilder::SetUrl(const string& url)
    {
        mUrl = url;
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
        char separator = '?';

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

        string link = mUrl + Slash + querry;

        return link;
    }
}
