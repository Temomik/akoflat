#include <gtest/gtest.h>

#include "OnlinerWebAdapter.h"

TEST(OnlinerWebAdapter, Initialize)
{
    Telegram::User::Config config;

    config.City = "Minsk";
    config.Price.first = 100;
    config.Price.second = 320;
    config.FloorCount = {2, 3, 4};

    Onliner::WebAdapter webAdapter;

    auto flats = webAdapter.GetFlats(config);

    ASSERT_TRUE(flats.size() != 0);
}
