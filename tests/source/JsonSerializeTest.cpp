#include <filesystem>
#include <gtest/gtest.h>
#include <string>

#include "UserConfig.h"
#include "TgUserConfigSerializator.h"

using std::string;

TEST(TELEGRAM_ID_CONFIG, IsSerializeAndDeserializeSuccess)
{
    Telegram::User::Config config;
    std::string raw = "{\"minPrice\":204,\"maxPrice\":762,\"city\":\"Brest\",\"floorCount\":[3,4],\"platforms\":[\"Onliner\",\"Realt\"]}";
    
    auto status = Serializator<Telegram::User::Config>::Deserialize(config, raw);
    auto serializeResult = Serializator<Telegram::User::Config>::Serialize(config);

    Telegram::User::Config resultConfig;
    Serializator<Telegram::User::Config>::Deserialize(resultConfig, serializeResult);

    EXPECT_EQ(resultConfig.City, config.City);
    EXPECT_EQ(resultConfig.FloorCount, config.FloorCount);
    EXPECT_EQ(resultConfig.Platforms, config.Platforms);
    EXPECT_EQ(resultConfig.Price, config.Price);

    EXPECT_TRUE(status);
}
