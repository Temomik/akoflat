#pragma once

#include "UserConfig.h"
#include <memory>
#include <string>

namespace Kufar
{
    class UrlBuilder
    {
    public:
        UrlBuilder() = default;
        bool BuildLink(const Telegram::User::Config& config);

        std::string GetUrl();
        std::string GetBaseUrl();

    private:
        static inline const std::string Platform = "Kufar";
        static inline const std::string BaseUrl = "https://re.kufar.by";
        static inline const std::string UrlModifier = "l";
        static inline const std::string SubPath = "snyat/kvartiru-dolgosrochno/bez-posrednikov";
        static inline const std::string RentTypeInitialQuerry = "rms=v.or%3A";
        static inline const std::string RentTypeMainQuerry = "%2C";
        static inline const std::string MinPriceQuerry = "prc=r%3A";
        static inline const std::string MaxPriceQuerry = "%2C";
        static inline const std::string CurrencyQuerry = "cur=USD";

        static constexpr size_t RentTypeConstrain = 5ul;
        static constexpr size_t MinPrice = 50ul;
        static constexpr size_t MaxPrice = 10000ul;

        std::string GetMaxPrice(const size_t price) const;
        std::string GetMinPrice(const size_t price) const;
        bool IsConfigValid(const Telegram::User::Config& config) const;

        std::shared_ptr<const std::string> mUrl;
    };
}
