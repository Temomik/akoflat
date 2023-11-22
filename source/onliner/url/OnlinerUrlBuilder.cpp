#include "OnlinerUrlBuilder.h"

#include "OnlinerPlacePresets.h"
#include "UrlBaseBuilder.h"

using Telegram::User::Config;
using std::string;
using Url::BaseBuilder;

namespace Onliner
{
    void UrlBuilder::BuildLink(const Telegram::User::Config& config)
    {
        //TODO validate config

        BaseBuilder urlBuilder;

        urlBuilder.SetUrl(BaseUrl);

        for (auto rentType : config.FloorCount)
        {
            if (rentType > 0 && rentType < RentTypeConstrain)
            {
                auto rentTypeItem = RentTypeOpen + std::to_string(rentType);

                if (rentType == 1)
                {
                    rentTypeItem += RentTypeCloseForOne;
                }
                else
                {
                    rentTypeItem += RentTypeClose;
                }

                urlBuilder.AddQuerryItem(rentTypeItem);
            }
        }

        urlBuilder.AddQuerryItem(MinPrice + std::to_string(config.Price.first));
        urlBuilder.AddQuerryItem(MaxPrice + std::to_string(config.Price.second));

        urlBuilder.AddQuerryItem(Currency);
        urlBuilder.AddQuerryItem(Owner);

        auto bounds = Onliner::Url::Presets::GetPresetForCity(config.City);
        urlBuilder.AddAnchorItem(bounds);

        mUrl = std::make_shared<const string>(urlBuilder.Build());
    }

    std::string UrlBuilder::GetNextPage()
    {
        return (*mUrl) + Page + std::to_string(mCurrentPage++);
    }
}
