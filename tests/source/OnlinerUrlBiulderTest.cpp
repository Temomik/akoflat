#include <filesystem>
#include <gtest/gtest.h>
#include <string>

#include "OnlinerUrlBuilder.h"

using std::string;

TEST(OnlinerUrlBuilder, BuildUrlFromValidConfig)
{
    Onliner::UrlBuilder builder;
    Telegram::User::Config config;

    string expectedLink = "https://r.onliner.by/ak/?rent_type%5B%5D=1_room&rent_type%5B%5D=2_rooms&rent_type%5B%5D=3_rooms&price%5Bmin%5D=100&price%5Bmax%5D=330&currency=usd&only_owner=true#bounds%5Blb%5D%5Blat%5D=53.8&bounds%5Blb%5D%5Blong%5D=27.39&bounds%5Brt%5D%5Blat%5D=54.01&bounds%5Brt%5D%5Blong%5D=27.72&page=1";

    config.City = "minsk";
    config.Price.first = 100;
    config.Price.second = 330;
    config.FloorCount = {1,2,3};

    builder.BuildLink(config);
    auto link = builder.GetNextPage();

    EXPECT_EQ(expectedLink, link);
}
