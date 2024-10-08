#include <gtest/gtest.h>

#ifdef WEB_TESTS

#include "OnlinerWebAdapter.h"

TEST(OnlinerWebAdapter, Initialize)
{
    Telegram::User::Config config;

    config.City = "Minsk";
    config.Price.first = 100;
    config.Price.second = 320;
    config.FloorCount = {2, 3, 4};
    config.Platforms = {"Onliner"};

    Onliner::WebAdapter webAdapter;

    auto flats = webAdapter.GetFlats(config);

    ASSERT_TRUE(flats.size() != 0);
}

#endif