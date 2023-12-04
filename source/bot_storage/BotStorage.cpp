#include "BotStorage.h"

#include <filesystem>

#include "TgUserConfigSerializator.h"

using std::string;
namespace fs = std::filesystem;

namespace
{
    static string ShownIdsFolder = "shown_flat_ids";
    static string KnownIdsFile = "known_ids";
    static string KnownIdsFolder = "ids";

    static string UserConfigFolder = "users_config";
}

BotStorage::BotStorage(const std::string& rootFolder)
    : mRootFolder(rootFolder), mPersistenceStorage(rootFolder)
{
}

void BotStorage::Init()
{
    std::scoped_lock lock(mIdMutex, mFlatIdMutex, mConfigdMutex);

    auto ids = mPersistenceStorage.GetLines(KnownIdsFolder, KnownIdsFile);

    for (auto& id : ids)
    {
        mIds.insert(id);

        auto lines = mPersistenceStorage.GetLines(ShownIdsFolder, id);
        std::set<std::string> shownFlats(lines.begin(), lines.end());

        mIdsData.insert(std::pair(id, shownFlats));
    }
}

std::deque<std::string> BotStorage::GetIds() const
{
    std::scoped_lock lock(mIdMutex);

    return std::deque<std::string>(mIds.begin(), mIds.end());
}

bool BotStorage::SaveId(const std::string& id)
{
    std::scoped_lock lock(mIdMutex);

    if (IsIdExist(id))
    {
        return true;
    }

    auto ret = SaveIdInternal(id);

    mIds.insert(id);
    mIdsData.insert(std::pair(id, std::set<std::string>()));

    return ret;
}

bool BotStorage::RemoveId(const std::string& id)
{
    std::scoped_lock lock(mIdMutex);

    if (!IsIdExist(id))
    {
        return false;
    }

    bool ret = true;
    mIds.erase(id);
    mIdsData.erase(id);

    fs::path path(KnownIdsFolder);
    path /= KnownIdsFile;

    ret &= mPersistenceStorage.RemoveAll(path);

    for (auto& id : mIds)
    {
        ret &= SaveIdInternal(id);
    }

    return ret;
}

std::optional<Telegram::User::Config> BotStorage::GetUserConfig(const std::string& id) const
{
    std::scoped_lock lock(mConfigdMutex);

    auto raw = mPersistenceStorage.Load(UserConfigFolder, id);
    Telegram::User::Config config;

    if (!raw.empty() && Serializator<Telegram::User::Config>::Deserialize(config, raw))
    {
        return config;
    }

    return {};
}

bool BotStorage::SaveUserConfig(const std::string& id, const Telegram::User::Config& config)
{
    std::scoped_lock lock(mConfigdMutex);

    auto raw = Serializator<Telegram::User::Config>::Serialize(config);

    return mPersistenceStorage.Save(UserConfigFolder, id, raw);
}

bool BotStorage::SaveShownFlatId(const std::string& id, const std::string& flatId)
{
    std::scoped_lock lock(mFlatIdMutex);

    if (IsFlatIdExist(id, flatId))
    {
        return false;
    }

    auto ret = SaveShownFlatIdInternal(id, flatId);
    mIdsData.find(id)->second.insert(flatId);

    return ret;
}

bool BotStorage::IsNewFlatId(const std::string& id, const std::string& flatId) const
{
    std::scoped_lock lock(mFlatIdMutex);

    return !IsFlatIdExist(id, flatId);
}

bool BotStorage::IsIdExist(const string& id) const
{
    return mIds.find(id) != mIds.end() &&
           mIdsData.find(id) != mIdsData.end();
}

bool BotStorage::IsFlatIdExist(const string& id, const string& flatId) const
{
    auto it = mIdsData.find(id);

    return mIds.find(id) != mIds.end() &&
           mIdsData.find(id) != mIdsData.end() &&
           it->second.find(flatId) != it->second.end();
}

bool BotStorage::SaveIdInternal(const string& id) const
{
    return mPersistenceStorage.Append(KnownIdsFolder, KnownIdsFile, id + "\n");
}

bool BotStorage::SaveShownFlatIdInternal(const string& id, const string& flatId) const
{
    return mPersistenceStorage.Append(ShownIdsFolder, id, flatId + "\n");
}
