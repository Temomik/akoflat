#include <UrlBaseBuilder.h>

namespace UrlBuilder
{
    void UrlBuilder::BaseBuilder::SetUrl(const string &url)
    {
        mUrl = url;
    }
    
    void BaseBuilder::AddQuerryItem(const string &item)
    {
        mQuerryItems.push_back(item);
    }

    void BaseBuilder::Reset()
    {
        mUrl.clear();
        mQuerryItems.clear();
    }
    const string BaseBuilder::Build() const
    {
        string querry;
        char separator = '/';
        
        for (size_t i = 0; i < mQuerryItems.size(); ++i)
        {
            querry += separator;            
            querry += mQuerryItems[i];

            separator = i == 0 ? '?' : '&';
        }

        string link = mUrl + querry;
        
        return link;
    }
}
