#include <filesystem>
#include <gtest/gtest.h>
#include <string>

#include "KufarUrlBuilder.h"

using std::string;

TEST(KufarUrlBuilder, BuildUrlFromValidConfig)
{
    Kufar::UrlBuilder builder;
    Telegram::User::Config config;

    string expectedLink = "https://re.kufar.by/l/minsk/snyat/kvartiru-dolgosrochno/bez-posrednikov/?cur=USD&prc=r%3A50%2C300&rms=v.or%3A2%2C3%2C4%2C5";

    config.City = "minsk";
    config.Price.first = 50;
    config.Price.second = 300;
    config.FloorCount = {2, 3, 4, 5};
    config.Platforms = {"Kufar"};

    builder.BuildLink(config);
    auto link = builder.GetUrl();

    EXPECT_EQ(expectedLink, link);
}

TEST(KufarUrlBuilder, BuildUrlFromValidConfig_Round)
{
    Kufar::UrlBuilder builder;
    Telegram::User::Config config;

    string expectedLink = "https://re.kufar.by/l/minsk/snyat/kvartiru-dolgosrochno/bez-posrednikov/?cur=USD&prc=r%3A50%2C300&rms=v.or%3A2%2C3%2C4%2C5";

    config.City = "minsk";
    config.Price.first = 51;
    config.Price.second = 291;
    config.FloorCount = {2, 3, 4, 5};
    config.Platforms = {"Kufar"};

    builder.BuildLink(config);
    auto link = builder.GetUrl();

    EXPECT_EQ(expectedLink, link);
}

TEST(KufarUrlBuilder, BuildUrlFromValidConfig_Clamp)
{
    Kufar::UrlBuilder builder;
    Telegram::User::Config config;

    string expectedLink = "https://re.kufar.by/l/minsk/snyat/kvartiru-dolgosrochno/bez-posrednikov/?cur=USD&prc=r%3A50%2C10000&rms=v.or%3A2%2C3%2C4%2C5";

    config.City = "minsk";
    config.Price.first = 0;
    config.Price.second = 999999;
    config.FloorCount = {2, 3, 4, 5};
    config.Platforms = {"Kufar"};

    builder.BuildLink(config);
    auto link = builder.GetUrl();

    EXPECT_EQ(expectedLink, link);
}
