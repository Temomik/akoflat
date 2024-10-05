#pragma once

#include "UserConfig.h"
#include <memory>
#include <string>

namespace Onliner
{
    class UrlBuilder
    {
    public:
        UrlBuilder() = default;
        bool BuildLink(const Telegram::User::Config& config);

        std::string GetNextPage();

    private:
        static inline const std::string Platform = "Onliner";
        static inline const std::string BaseUrl = "https://r.onliner.by/ak";
        static inline const std::string RentTypeOpenQuerry = "rent_type%5B%5D=";
        static inline const std::string RentTypeCloseQuerry = "_rooms";
        static inline const std::string RentTypeCloseForOneQuerry = "_room";
        static inline const std::string MinPriceQuerry = "price%5Bmin%5D=";
        static inline const std::string MaxPriceQuerry = "price%5Bmax%5D=";
        static inline const std::string CurrencyQuerry = "currency=usd";
        static inline const std::string OwnerQuerry = "only_owner=true";
        static inline const std::string PageQuerry = "&page=";

        static constexpr size_t RentTypeConstrain = 7ul;
        static constexpr size_t MinPrice = 50ul;
        static constexpr size_t MaxPrice = 10000ul;

        std::string GetMaxPrice(const size_t price) const;
        std::string GetMinPrice(const size_t price) const;
        bool IsConfigValid(const Telegram::User::Config& config) const;

        std::shared_ptr<const std::string> mUrl;
        size_t mCurrentPage = 1;
    };
}
