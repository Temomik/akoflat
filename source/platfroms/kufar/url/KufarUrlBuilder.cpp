#include "KufarUrlBuilder.h"

#include "KufarPlacePresets.h"
#include "UrlBaseBuilder.h"

#include <algorithm>
#include <cmath>

using std::string;
using Telegram::User::Config;
using Url::BaseBuilder;

namespace Kufar
{
    bool UrlBuilder::BuildLink(const Telegram::User::Config& config)
    {
        auto bounds = Kufar::Url::Presets::GetPresetForCity(config.City);

        if (bounds.empty() || !IsConfigValid(config))
        {
            return false;
        }

        BaseBuilder urlBuilder;

        urlBuilder.SetUrl(BaseUrl);

        bool isFirst = true;
        string rentQuerry;
        for (auto rentType : config.FloorCount)
        {
            if (isFirst)
            {
                isFirst = false;
                rentQuerry += RentTypeInitialQuerry + std::to_string(rentType);
            }
            else
            {
                rentQuerry += RentTypeMainQuerry + std::to_string(rentType);
            }
        }

        auto priceQuerry = MinPriceQuerry + GetMinPrice(config.Price.first) + MaxPriceQuerry + GetMaxPrice(config.Price.second);

        urlBuilder.AddQuerryItem(CurrencyQuerry);
        urlBuilder.AddQuerryItem(priceQuerry);
        urlBuilder.AddQuerryItem(rentQuerry);

        urlBuilder.AddSubPathItem(UrlModifier);
        urlBuilder.AddSubPathItem(bounds);
        urlBuilder.AddSubPathItem(SubPath);

        mUrl = std::make_shared<const string>(urlBuilder.Build());

        return true;
    }

    std::string UrlBuilder::GetUrl()
    {
        return *mUrl;
    }

    std::string UrlBuilder::GetBaseUrl()
    {
        return BaseUrl;
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

        return isCurrentPlatform && !config.City.empty() && !config.FloorCount.empty() && config.Price.first < config.Price.second;
    }
}
