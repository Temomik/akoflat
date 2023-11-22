#pragma once

#include "UserConfig.h"
#include <string>
#include <memory>

namespace Onliner
{
    class UrlBuilder
    {
    public:
        UrlBuilder() = default;
        void BuildLink(const Telegram::User::Config& config);

        std::string GetNextPage();

    private:
        static inline const std::string BaseUrl = "https://r.onliner.by/ak";
        static inline const std::string RentTypeOpen = "rent_type%5B%5D=";
        static inline const std::string RentTypeClose = "_rooms";
        static inline const std::string RentTypeCloseForOne = "_room";
        static inline const std::string MinPrice = "price%5Bmin%5D=";
        static inline const std::string MaxPrice = "price%5Bmax%5D=";
        static inline const std::string Currency = "currency=usd";
        static inline const std::string Owner = "only_owner=true";    
        static inline const std::string Page = "&page=";    

        static constexpr size_t RentTypeConstrain = 7;

        std::shared_ptr<const std::string> mUrl;
        size_t mCurrentPage = 1;
    };
}
