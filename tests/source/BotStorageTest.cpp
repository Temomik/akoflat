#include <gtest/gtest.h>

#include "BotStorage.h"
#include "PersistenceStorage.h"

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

TEST(BotStorage, Remove_WhenDoesNotExist_ShouldNotBeRmoved)
{
    PersistenceStorage persistenceStorage(rootPath);
    persistenceStorage.RemoveAll("");

    const string id = "432101234";
    size_t localIdsCount = 0, fsIdsCount = 0;

    BotStorage storage(rootPath);
    storage.Init();

    auto status = storage.RemoveId(id);

    localIdsCount = storage.GetIds().size();

    EXPECT_FALSE(status);
}

TEST(BotStorage, SaveShownFlatId_SaveNewFlatId_ShouldBeSaved)
{
    PersistenceStorage persistenceStorage(rootPath);
    persistenceStorage.RemoveAll("");

    const string id = "432101234";
    const string flatId = "flatId";
    string localFlatId, fsFlatId;

    {
        BotStorage storage(rootPath);
        storage.Init();

        storage.SaveId(id);
        storage.SaveShownFlatId(id, flatId);

        localFlatId = storage.GetShownFlatIds(id).value().front();
    }
    {
        BotStorage storage(rootPath);
        storage.Init();

        fsFlatId = storage.GetShownFlatIds(id).value().front();
    }

    EXPECT_EQ(localFlatId, flatId);
    EXPECT_EQ(fsFlatId, flatId);
}
