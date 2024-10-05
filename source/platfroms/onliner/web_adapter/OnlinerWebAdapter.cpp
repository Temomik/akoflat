#include "OnlinerWebAdapter.h"

#include "HtmlRequester.h"
#include "Logger.h"
#include "OnlinerHtmlParser.h"
#include "OnlinerUrlBuilder.h"
#include <iostream>

Onliner::WebAdapter::WebAdapter(size_t retryCount)
    : mRetryCount(retryCount)
{
}

std::vector<Telegram::FlatDto> Onliner::WebAdapter::GetFlats(const Telegram::User::Config& config) const
{
    size_t maxPage = 1;

    Onliner::UrlBuilder builder;
    Html::HtmlRequester requester;
    std::vector<Telegram::FlatDto> flats;

    if (!builder.BuildLink(config))
    {
        return flats;
    }

    for (size_t retry = 0; retry < mRetryCount; retry++)
    {
        for (size_t i = 0; i < maxPage; i++)
        {
            auto link = builder.GetNextPage();
            auto htmlCode = requester.Get(link);

            if (htmlCode->size() == 0)
            {
                flats.clear();
                LOG_WARNING("Failed to load page");
                continue;
            }

            Onliner::HtmlParser parser(htmlCode);

            auto status = parser.Parse(flats);

            if (!status)
            {
                flats.clear();
                LOG_WARNING("Failed to parse page");
                continue;
            }

            maxPage = parser.GetPagesCount();
        }

        if (!flats.empty())
        {
            break;
        }
    }

    return flats;
}
