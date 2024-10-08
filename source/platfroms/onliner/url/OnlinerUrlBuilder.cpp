#include "OnlinerUrlBuilder.h"

#include "OnlinerPlacePresets.h"
#include "UrlBaseBuilder.h"

#include <algorithm>
#include <cmath>

using std::string;
using Telegram::User::Config;
using Url::BaseBuilder;

namespace Onliner
{
    bool UrlBuilder::BuildLink(const Telegram::User::Config& config)
    {
        auto bounds = Onliner::Url::Presets::GetPresetForCity(config.City);

        if (bounds.empty() || !IsConfigValid(config))
        {
            return false;
        }

        BaseBuilder urlBuilder;

        urlBuilder.SetUrl(BaseUrl);

        for (auto rentType : config.FloorCount)
        {
            if (rentType > 0 && rentType < RentTypeConstrain)
            {
                auto rentTypeItem = RentTypeOpenQuerry + std::to_string(rentType);

                if (rentType == 1)
                {
                    rentTypeItem += RentTypeCloseForOneQuerry;
                }
                else
                {
                    rentTypeItem += RentTypeCloseQuerry;
                }

                urlBuilder.AddQuerryItem(rentTypeItem);
            }
        }

        urlBuilder.AddQuerryItem(MinPriceQuerry + GetMinPrice(config.Price.first));
        urlBuilder.AddQuerryItem(MaxPriceQuerry + GetMaxPrice(config.Price.second));

        urlBuilder.AddQuerryItem(CurrencyQuerry);
        urlBuilder.AddQuerryItem(OwnerQuerry);

        urlBuilder.AddAnchorItem(bounds);

        mUrl = std::make_shared<const string>(urlBuilder.Build(true));

        return true;
    }

    std::string UrlBuilder::GetNextPage()
    {
        return (*mUrl) + PageQuerry + std::to_string(mCurrentPage++);
    }

    std::string UrlBuilder::GetMaxPrice(const size_t price) const
    {
        size_t ceilPrice = std::ceil(price / 10.) * 10ul;
        size_t clampPrice = std::clamp(ceilPrice, MinPrice, MaxPrice);

        return std::to_string(clampPrice);
    }

    std::string UrlBuilder::GetMinPrice(const size_t price) const
    {
        size_t ceilPrice = std::floor(price / 10.) * 10ul;
        size_t clampPrice = std::clamp(ceilPrice, MinPrice, MaxPrice);

        return std::to_string(clampPrice);
    }

    bool UrlBuilder::IsConfigValid(const Telegram::User::Config& config) const
    {
        bool isCurrentPlatform = false;
        for (auto& platfrom : config.Platforms)
        {
            if (Utils::EqualsIC(platfrom, Platform, Platform.size()))
            {
                isCurrentPlatform = true;
                break;
            }
        }

        return isCurrentPlatform 
            && !config.City.empty() 
            && !config.FloorCount.empty() 
            && config.Price.first < config.Price.second;
    }
}
