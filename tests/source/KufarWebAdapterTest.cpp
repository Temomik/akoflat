#include <gtest/gtest.h>

#define WEB_TESTS

#ifdef WEB_TESTS

#include "KufarWebAdapter.h"

TEST(OnlinerWebAdapter, Initialize)
{
    Telegram::User::Config config;

    config.City = "Minsk";
    config.Price.first = 100;
    config.Price.second = 320;
    config.FloorCount = {2, 3, 4};    
    config.Platforms = {"Kufar"};

    Kufar::WebAdapter webAdapter(15);

    auto flats = webAdapter.GetFlats(config);

    ASSERT_TRUE(flats.size() != 0);
}

#endif