#include <gtest/gtest.h>

#include "BotStorage.h"
#include "PersistenceStorage.h"
#include "TgUserConfigSerializator.h"

using std::string;

namespace
{
    static string rootPath = "bot_storage_test";
}

TEST(BotStorage, Init_WhenAllEmpty_ShouldBeEmpty)
{
    BotStorage storage(rootPath);
    PersistenceStorage persistenceStorage(rootPath);

    persistenceStorage.RemoveAll("");
    storage.Init();

    EXPECT_EQ(storage.GetIds().size(), 0);
}

TEST(BotStorage, SaveId_WhenNewId_ShouldBeSaved)
{
    PersistenceStorage persistenceStorage(rootPath);
    persistenceStorage.RemoveAll("");

    const string id = "432101234";
    string localId, idFromFs;

    {
        BotStorage storage(rootPath);
        storage.Init();
        storage.SaveId(id);

        localId = storage.GetIds().front();
    }

    {
        BotStorage storage(rootPath);
        storage.Init();

        idFromFs = storage.GetIds().front();
    }

    EXPECT_EQ(localId, id);
    EXPECT_EQ(idFromFs, id);
}

TEST(BotStorage, Remove_WhenExist_ShouldBeRmoved)
{
    PersistenceStorage persistenceStorage(rootPath);
    persistenceStorage.RemoveAll("");

    const string id = "432101234";
    size_t localIdsCount = 0, fsIdsCount = 0;
    bool status = false;

    {
        BotStorage storage(rootPath);
        storage.Init();

        storage.SaveId(id);
        status = storage.RemoveId(id);

        localIdsCount = storage.GetIds().size();
    }
    {
        BotStorage storage(rootPath);
        storage.Init();

        fsIdsCount = storage.GetIds().size();
    }

    EXPECT_TRUE(status);
    EXPECT_EQ(fsIdsCount, 0);
    EXPECT_EQ(localIdsCount, 0);
}

TEST(BotStorage, Remove_WhenDoesNotExist_ShouldNotBeRemoved)
{
    PersistenceStorage persistenceStorage(rootPath);
    persistenceStorage.RemoveAll("");

    const string id = "432101234";

    BotStorage storage(rootPath);
    storage.Init();

    auto status = storage.RemoveId(id);

    EXPECT_FALSE(status);
}

TEST(BotStorage, SaveShownFlatId_SaveNewFlatId_ShouldBeSaved)
{
    PersistenceStorage persistenceStorage(rootPath);
    persistenceStorage.RemoveAll("");

    const string id = "432101234";
    const string flatId = "flatId";

    {
        BotStorage storage(rootPath);
        storage.Init();

        storage.SaveId(id);
        storage.SaveShownFlatId(id, flatId);
        
        EXPECT_FALSE(storage.IsNewFlatId(id, flatId));
    }
    {
        BotStorage storage(rootPath);
        storage.Init();

        EXPECT_FALSE(storage.IsNewFlatId(id, flatId));
    }
}

TEST(BotStorage, SaveLoadConfig_WhenDoesNotExist_ShouldBeSuccess)
{
    PersistenceStorage persistenceStorage(rootPath);
    persistenceStorage.RemoveAll("");

    const string id = "432101234";
    std::string raw = "{\"minPrice\":204,\"maxPrice\":762,\"city\":\"Brest\",\"floorCount\":[3,4],\"platforms\":[\"Onliner\",\"Realt\"]}";
    Telegram::User::Config config;

    Serializator<Telegram::User::Config>::Deserialize(config, raw);

    BotStorage storage(rootPath);
    storage.Init();

    auto status = storage.SaveUserConfig(id, config);
    auto loadConfig = storage.GetUserConfig(id).value();

    EXPECT_EQ(loadConfig.City, config.City);
    EXPECT_EQ(loadConfig.FloorCount, config.FloorCount);
    EXPECT_EQ(loadConfig.Platforms, config.Platforms);
    EXPECT_EQ(loadConfig.Price, config.Price);

    EXPECT_TRUE(status);
}
