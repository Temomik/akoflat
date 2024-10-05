#include "KufarWebAdapter.h"

#include "HtmlRequester.h"
#include "Logger.h"
#include "KufarHtmlParser.h"
#include "KufarUrlBuilder.h"
#include <iostream>

Kufar::WebAdapter::WebAdapter(size_t retryCount)
    : mRetryCount(retryCount)
{
}

std::vector<Telegram::FlatDto> Kufar::WebAdapter::GetFlats(const Telegram::User::Config& config) const
{
    Kufar::UrlBuilder builder;
    Html::HtmlRequester requester;
    std::vector<Telegram::FlatDto> flats;

    if (!builder.BuildLink(config))
    {
        return flats;
    }

    auto link = builder.GetUrl();
    for (size_t retry = 0; retry < mRetryCount; retry++)
    {
        for (size_t pageNumber = 2;; pageNumber++)
        {
            auto htmlCode = requester.Get(link);

            if (htmlCode->size() == 0)
            {
                flats.clear();
                LOG_WARNING("Failed to load page");
                continue;
            }

            Kufar::HtmlParser parser(htmlCode);

            auto status = parser.Parse(flats);
            link = parser.GetPageLink(pageNumber);

            if (link.empty())
            {
                return flats;
            }

            link = builder.GetBaseUrl() + link;

            if (!status)
            {
                flats.clear();
                LOG_WARNING("Failed to parse page");
                continue;
            }
        }

        if (!flats.empty())
        {
            break;
        }
    }

    return flats;
}
